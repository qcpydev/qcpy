#define _GNU_SOURCE
#include <block.h>
#include <complex.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

#pragma once
#ifndef DOCK_H
#define DOCK_H

// turn importer structs here, create block directory which will own a block
// request

int dock_add(block_t *block);
void dock_run_boot();

char *dock_init(char *args[]);
char *dock_quack_init(char *args[]);
char *dock_quack_gpu_init(char *args[]);
bool dock_set_gpu_enabled(char *args[]);

void dock_port_init();
void dock_wait_for_boot();

int dock_get_qc_entries(uint32_t reg, block_t *blocks);
void dock_free_qc_entries(block_t *blocks);

typedef struct boot_thread_args_s {
  char *exec_name;
  char *quack_exec_name;
  char *quack_gpu_exec_name;
  bool is_gpu_enabled;
  char **args;
} boot_thread_args_t;

extern boot_thread_args_t *boot_thread_args;

#endif // DOCK_H
