#pragma once

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_FONT_BOLD "\x1b[1m"

void pred(const char *msg, ...);
void pblue(const char *msg, ...);
void pgrn(const char *msg, ...);
void pylw(const char *msg, ...);
void pcyn(const char *msg, ...);
void pmgn(const char *msg, ...);

#ifdef DEBUG
#define dbg(x, ...)                                                           \
	dbg2(ANSI_FONT_BOLD "<%s:%d:%s> " ANSI_COLOR_RESET x, __FILE__, __LINE__, \
	     __func__, ##__VA_ARGS__)
void dbg2(const char *msg, ...);

#define err(x, ...)                                                           \
	err2(ANSI_FONT_BOLD "<%s:%d:%s> " ANSI_COLOR_RESET x, __FILE__, __LINE__, \
	     __func__, ##__VA_ARGS__)
void err2(const char *msg, ...);

#define info(x, ...)                                                           \
	info2(ANSI_FONT_BOLD "<%s:%d:%s> " ANSI_COLOR_RESET x, __FILE__, __LINE__, \
	      __func__, ##__VA_ARGS__)
void info2(const char *msg, ...);

#define warn(x, ...)                                                           \
	warn2(ANSI_FONT_BOLD "<%s:%d:%s> " ANSI_COLOR_RESET x, __FILE__, __LINE__, \
	      __func__, ##__VA_ARGS__)
void warn2(const char *msg, ...);
#else
void dbg(const char *msg, ...);
void err(const char *msg, ...);
void info(const char *msg, ...);
void warn(const char *msg, ...);
#endif
