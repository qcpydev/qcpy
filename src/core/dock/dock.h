#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <block.h>

#pragma once
#ifndef DOCK_H
#define DOCK_H

// turn importer structs here, create block directory which will own a block request

int dock_add(block_t* block);
void* dock_run_boot(void*);
char* dock_init(char* args[]);
void dock_port_init();

typedef struct boot_thread_args_s {
  char* exec_name;
  char** args;
} boot_thread_args_t;


extern boot_thread_args_t* boot_thread_args;

#endif // DOCK_H
