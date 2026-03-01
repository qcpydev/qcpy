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

typedef struct dock_log_item_t dock_log_item_t;
struct dock_log_item_t {
  block_t block;
  dock_log_item_t *next;
};

typedef struct dock_log_t dock_log_t;
struct dock_log_t {
  dock_log_item_t *queue;
  dock_log_t *next;
  dock_log_item_t *last;
  dock_log_item_t *checkpoint;
  uint32_t reg;
};

typedef struct dock_logger_s {
  dock_log_t *logs;
  dock_log_t *last;
  dock_log_t *last_used;
} dock_logger_t;

dock_log_t *dock_log_init(uint32_t reg, block_t *block);
dock_log_item_t *dock_log_item_init(block_t *block);
void dock_log_append(uint32_t reg, block_t *block);

dock_log_t *dock_log_find(uint32_t reg);

int dock_log_fill_array(dock_log_t *dock_log, block_t *block);

extern dock_logger_t dock_log;

#endif // DOCK_H
