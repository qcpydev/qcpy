#include <block.h>
#include <stdlib.h>

#pragma once
#ifndef BASE_TOOLS_H
#define BASE_TOOLS_H

void *base_tools_create_shared_mem(size_t size, char *name);
sem_t *base_tools_create_shared_sem(char *name, unsigned int val);

void *base_tools_open_shared_mem(size_t, char *name);
sem_t *base_tools_open_shared_sem(char *name);

#endif
