/* 
 * Copyright (c) 2017, Jack Lange <jacklange@cs.pitt.edu>
 * All rights reserved.
 *
 * This is free software.  You are permitted to use,
 * redistribute, and modify it as specified in the file "PETLAB_LICENSE".
 */
#pragma once

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define log_error(fmt, ...) fprintf(stderr, "error> %s(%d): " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#ifdef DEBUG
#define log_debug(fmt, ...) fprintf(stderr, "debug> " fmt, ##__VA_ARGS__)
#else
#define log_debug(fmt, ...)
#endif

#define nexus_printf(fmt, ...) fprintf(stdout, "Nexus> " fmt, ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "\33[32mINFO\33[39m  " M "  \33[90m at %s (%s:%d) \33[39m\n", ##__VA_ARGS__, __func__, __FILE__, __LINE__)
