/*
**Original from http://c.learncodethehardway.org/book/ex20.html
**/

#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "\x1B[36m" "DEBUG %s:%d: " M "\n" "\x1B[37m", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "\x1B[31m" "[ERROR] (%s:%d: errno: %s) " M "\n" "\x1B[37m", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "\x1B[33m" "[WARN] (%s:%d: errno: %s) " M "\n" "\x1B[37m", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "\x1B[32m" "[INFO] (%s:%d) " M "\n" "\x1B[37m", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; }

#endif