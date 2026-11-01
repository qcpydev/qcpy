#define _GNU_SOURCE
#include <block.h>
#include <complex.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <uuid/uuid.h>

#pragma once
#ifndef DOCK_H
#define DOCK_H

/*
 * Dock has the core goal of dispatching data for other processes to consume. It
 * also has the ability to signal another process to handle different tasks,
 * such as quack_core to do calculations on a quantum circuit.
 * There is a thread pool that live on the process. The main Python/ctypes
 * thread will enqueue the block onto a thread via an LL, in which the
 */

void dock_add(block_t *block);
void dock_run_boot();

void dock_setup_global_shared_mem();
void dock_wait_for_qcpy_core_boot();

typedef enum {
  QCPY_BOOTARG_CORE_EXEC_NAME,
  QCPY_BOOTARG_QUACK_EXEC_NAME,
  QCPY_BOOTARG_QUACK_GPU_EXEC_NAME,
  QCPY_BOOTARG_IS_GPU_ENABLED,
} boot_args_e;

typedef struct bootarg_wrapper_s {
  char *qcpy_core_exec_name;
  char *quack_exec_name;
  char *quack_gpu_exec_name;
  bool is_gpu_enabled;
} bootarg_wrapper_t;

extern bootarg_wrapper_t qcpy_bootarg_wrapper;

typedef struct dock_queue_item_t dock_queue_item_t;

struct dock_queue_item_t {
  block_t *block;
  dock_queue_item_t *next;
};

typedef struct dock_queue_s {
  dock_queue_item_t *head;
  dock_queue_item_t *tail;
  size_t item_count;
  pthread_mutex_t lock;
  pthread_cond_t cond;
} dock_queue_t;

#define DOCK_QUEUES_TOTAL (8)

void dock_queue_init(dock_queue_t *dock_queue);
void dock_enqueue(block_t *block);
block_t *dock_dequeue(dock_queue_t *dock_queue);
void dock_setup_early_init(char *args[]);

#endif // DOCK_H
