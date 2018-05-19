#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "dump.h"
#include "display.h"
#include "common.h"

static i64 *dataset = NULL;
static siz size = 0;

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

static void dump_handler(int signal){
    switch(signal){
        case SIGSEGV:
            err("Caught SIGSEGV! Dumping data!");
            break;
        case SIGABRT:
            err("Caught SIGABRT! Dumping data!");
            break;
    }
    if(size > 0)
        dump_data_auto();
}

static void dump_free(){
    if(dataset != NULL)
        free(dataset);
}

void dump_init(){
    if(signal(SIGSEGV, dump_handler) == SIG_ERR)
        err("Unable to register SIGSESGV handler!");
    if(signal(SIGABRT, dump_handler) == SIG_ERR)
        err("Unable to register SIGABRT handler!");
    atexit(dump_free);
}
