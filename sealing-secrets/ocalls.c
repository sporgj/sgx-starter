/**
 * Copyright (c) 2018, Judicael Djoko <jbriand@cs.pitt.edu>
 *
 * This is Free Sotfware.
 */

#include "untrusted_header.h"

// -------------------------- utilities -----------------------

void *
ocall_calloc(size_t size)
{
    return nexus_malloc(size);
}

void
ocall_free(void * ptr)
{
    free(ptr);
}

void
ocall_print(char * str)
{
    printf("%s", str);
    fflush(stdout);
}
