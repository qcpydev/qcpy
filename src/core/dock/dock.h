#define _GNU_SOURCE
#include <block.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

#pragma once
#ifndef DOCK_H
#define DOCK_H

// turn importer structs here, create block directory which will own a block request

int dock_add(block_t* block);
void* dock_run_boot(void*);
char* dock_init(char* args[]);
void dock_port_init();
void dock_wait_for_boot();
void dock_get_qc_state(int flush_id, bool is_print);

typedef struct boot_thread_args_s
{
    char* exec_name;
    char** args;
} boot_thread_args_t;


extern boot_thread_args_t* boot_thread_args;

#endif // DOCK_H
