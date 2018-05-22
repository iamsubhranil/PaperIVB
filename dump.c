#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "dump.h"
#include "display.h"
#include "common.h"

#ifdef STACKTRACE_SHOW
#include <execinfo.h>
#endif

static i64 *dataset = NULL;
static siz size = 0;
static volatile sig_atomic_t sig_in_progress = 0;

void dump_set(i64 *arr, siz s){
    if(dataset != NULL)
        free(dataset);
    dataset = (i64 *)malloc(sizeof(i64) * s);
    memcpy(dataset, arr, s*sizeof(i64));
    size = s;
}

DumpData dump_get(){
    return (DumpData){dataset, size};
}

void dump_load(const char *name){
    FILE *f = fopen(name, "r");
    if(!f){
        err("No such dump file found!");
        return;
    }
    fscanf(f, "%" Psiz, &size);
    dataset = (i64 *)malloc(sizeof(i64) * size);
    for(siz i = 0;i < size;i++)
        fscanf(f, "%" Si64, &dataset[i]);
    fclose(f);
    info("Dump %s loaded successfully!", name);
}

void dump_load_last(){
    FILE *f = fopen(".lastdump", "r");
    if(!f){
        err("No .lastdump file found! Unable to open last dump!");
        return;
    }
    siz s;
    fscanf(f, "%" Ssiz, &s);
    char name[s+1];
    fscanf(f, "%s", name);
    name[s] = '\0';
    fclose(f);
    dump_load(name);
}

void dump_data(const char *name){
    FILE *f = fopen(name, "w");
    if(!f){
        err("Unable to write crash dump!");
        return;
    }

    fprintf(f, "%" Psiz, size);
    for(siz i = 0;i < size;i++)
        fprintf(f, " %" Pi64, dataset[i]);
    fclose(f);

    FILE *f2 = fopen(".lastdump", "w");
    if(!f2){
        err("Unable to mark dump as lastdump!");
    }
    else{
        siz s = strlen(name);
        fprintf(f, "%" Psiz " ", s);
        fprintf(f, "%s", name);
        fclose(f2);
    }
    info("Dump written successfully!\n");
}

void dump_data_append(const char *name){
    char *n = strdup(name);
    n = (char *)realloc(n, strlen(name) + 17);

    time_t rt;
    time(&rt);
    struct tm* time = localtime(&rt);

    //crashdump_20180519_191753
    char *t = (char *)malloc(22);
    strftime(t, 26, "_%Y%m%d_%H%M%S.dump", time);
    t[21] = '\0';

    strcat(n, t);

    dump_data(n);

    free(n);
    free(t);
}

static void dump_data_auto(){
    dump_data_append("crashdump");
}

static void dump_free(){
    if(dataset != NULL)
        free(dataset);
}

// From https://spin.atomicobject.com/2013/01/13/exceptions-stack-traces-c/

#ifdef STACKTRACE_SHOW

typedef struct{
    char *name, *offset, *address;
} ProgramInfo;

static ProgramInfo get_program_info(const char *address){
    siz i = 0;
    ProgramInfo p;
    while(address[i] != '(' && address[i + 1] != '+' && address[i+2] != '0'
            && address[i] != 'x')
        i++;
    char *name = (char *)malloc(i + 1);
    strncpy(name, address, i);
    name[i] = '\0';
    p.name = name;

    siz bak = i + 1;
    while(address[i] != ')')
        i++;
    char *offset = (char *)malloc(i - bak + 2);
    for(siz tmp = bak, idx = 0;tmp < i;tmp++, idx++)
        offset[idx] = address[tmp];
    offset[i - bak + 1] = '\0';
    p.offset = offset;
    return p;
}

static u64 stack_count = 0;

/* Resolve symbol name and source location given the path to the executable
   and an address */
static int addr2line(const char *addr){
    char addr2line_cmd[512] = {0};

    ProgramInfo info = get_program_info(addr);
    /* have addr2line map the address to the relent line in the code */
#ifdef __APPLE__
    /* apple does things differently... */
    sprintf(addr2line_cmd,"atos -o %s %s", info.name, info.offset);
#else
    sprintf(addr2line_cmd,"addr2line -f -i -p -e %s %s", info.name, info.offset);
#endif

    free(info.name);
    free(info.offset);

    /* This will print a nicely formatted string specifying the
       function and source line of the address */
    printf(ANSI_COLOR_RED ANSI_FONT_BOLD "[#%" Pi64 "] " ANSI_COLOR_RESET, stack_count--);
    fflush(stdout);
    int i = system(addr2line_cmd);
    fflush(stdout);
    return i;
}

#define MAX_STACK_FRAMES 64
static void *stack_traces[MAX_STACK_FRAMES];

static void posix_print_stack_trace(){
    int i, trace_size = 0;
    char **messages = (char **)NULL;

    trace_size = backtrace(stack_traces, MAX_STACK_FRAMES);
    stack_count = trace_size - 6;
    messages = backtrace_symbols(stack_traces, trace_size);

    /* skip the first couple stack frames (as they are this function and
       our handler) and also skip the last frame as it's (always?) junk. */
    // for (i = 3; i < (trace_size - 1); ++i)
    // we'll use this for now so you can see what's going on
    for (i = 3; i < (trace_size - 2); ++i){
        fflush(stdout);
        if (addr2line(messages[i]) != 0){
            err("At %s %p", messages[i], stack_traces[i]);
        }

    }
    if (messages) { free(messages); }
}

#endif

static void posix_signal_handler(int sig, siginfo_t *siginfo, void *context){
    if(sig_in_progress)
        return;
    sig_in_progress = 1;
    (void)context;
    printf("\n");
    switch(sig){
        case SIGSEGV:
            err("Caught SIGSEGV: Segmentation Fault!");
            break;
        case SIGINT:
            err("Caught SIGINT: Interactive attention signal (usually ctrl+c)!");
            break;
        case SIGFPE:
            switch(siginfo->si_code){
                case FPE_INTDIV:
                    err("Caught SIGFPE: Integer divide by zero!");
                    break;
                case FPE_INTOVF:
                    err("Caught SIGFPE: Integer overflow!");
                    break;
                case FPE_FLTDIV:
                    err("Caught SIGFPE: Floating-point divide by zero!");
                    break;
                case FPE_FLTOVF:
                    err("Caught SIGFPE: Floating-point overflow!");
                    break;
                case FPE_FLTUND:
                    err("Caught SIGFPE: Floating-point underflow!");
                    break;
                case FPE_FLTRES:
                    err("Caught SIGFPE: Floating-point inexact result!");
                    break;
                case FPE_FLTINV:
                    err("Caught SIGFPE: Floating-point invalid operation!");
                    break;
                case FPE_FLTSUB:
                    err("Caught SIGFPE: (subscript out of range!");
                    break;
                default:
                    err("Caught SIGFPE: Arithmetic Exception!");
                    break;
            }
        case SIGILL:
            switch(siginfo->si_code){
                case ILL_ILLOPC:
                    err("Caught SIGILL: Illegal opcode!");
                    break;
                case ILL_ILLOPN:
                    err("Caught SIGILL: Illegal operand!");
                    break;
                case ILL_ILLADR:
                    err("Caught SIGILL: Illegal addressing mode!");
                    break;
                case ILL_ILLTRP:
                    err("Caught SIGILL: Illegal trap!");
                    break;
                case ILL_PRVOPC:
                    err("Caught SIGILL: Privileged opcode!");
                    break;
                case ILL_PRVREG:
                    err("Caught SIGILL: Privileged register!");
                    break;
                case ILL_COPROC:
                    err("Caught SIGILL: Coprocessor error!");
                    break;
                case ILL_BADSTK:
                    err("Caught SIGILL: Internal stack error!");
                    break;
                default:
                    err("Caught SIGILL: Illegal Instruction!");
                    break;
            }
            break;
        case SIGTERM:
            err("Caught SIGTERM: A termination request was sent to the program!");
            break;
        case SIGABRT:
            err("Caught SIGABRT: Usually caused by an abort() or assert()!");
            break;
        default:
            break;
    }
    printf("\n");
#ifdef STACKTRACE_SHOW
    posix_print_stack_trace();
#else
    warn("Stacktrace is not currently available for this platform!");
#endif
    if(size > 0)
        dump_data_auto();
    _Exit(1);
}

static uint8_t alternate_stack[SIGSTKSZ];
static void set_signal_handler(){
    /* setup alternate stack */
    {
        stack_t ss = {};
        /* malloc is usually used here, I'm not 100% sure my static allocation
           is valid but it seems to work just fine. */
        ss.ss_sp = (void*)alternate_stack;
        ss.ss_size = SIGSTKSZ;
        ss.ss_flags = 0;

        if (sigaltstack(&ss, NULL) != 0) { err("Failed to set alternate sigaltstack!"); }
    }

    /* register our signal handlers */
    {
        struct sigaction sig_action = {};
        sig_action.sa_sigaction = posix_signal_handler;
        sigemptyset(&sig_action.sa_mask);

#ifdef __APPLE__
        /* for some reason we backtrace() doesn't work on osx
           when we use an alternate stack */
        sig_action.sa_flags = SA_SIGINFO;
#else
        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
#endif

#define REG_HAND(x) \
        if(sigaction(x, &sig_action, NULL) != 0) err("Unable to register handler for " #x "!")

        REG_HAND(SIGSEGV);
        REG_HAND(SIGFPE);
        REG_HAND(SIGINT);
        REG_HAND(SIGILL);
        REG_HAND(SIGTERM);
        REG_HAND(SIGABRT);

#undef REG_HAND
    }
}

void dump_init(){
    set_signal_handler();
    atexit(dump_free);
}
