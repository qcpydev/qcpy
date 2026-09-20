#include <block.h>
#include <stdlib.h>

#pragma once
#ifndef BASE_TOOLS_H
#define BASE_TOOLS_H

int base_tools_create_shared_mem(size_t size, char *name, void **mem);
sem_t *base_tools_create_shared_sem(char *name, unsigned int val);

#endif
