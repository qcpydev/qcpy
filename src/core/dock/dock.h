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

/*
 * Dock has the core goal of dispatching data for other processes to consume. It
 * also has the ability to signal another process to handle different tasks,
 * such as quack_core to do calculations on a quantum circuit.
 * There is a thread pool that live on the process. The main Python/ctypes
 * thread will enqueue the block onto a thread via an LL, in which the
 */
typedef struct dock_add_params_s {
  uint64_t *qubits;
  uint64_t *controlled_qubits;
  uint64_t *target_qubits;
} dock_add_params_t;

int dock_add(block_t *block);
void dock_run_boot();

char *dock_init(char *args[]);
char *dock_quack_init(char *args[]);
char *dock_quack_gpu_init(char *args[]);
bool dock_set_gpu_enabled(char *args[]);

void dock_port_init();
void dock_wait_for_boot();

void dock_free_qc_entries(block_t *blocks);

typedef struct boot_thread_args_s {
  char *exec_name;
  char *quack_exec_name;
  char *quack_gpu_exec_name;
  bool is_gpu_enabled;
  char **args;
} boot_thread_args_t;

extern boot_thread_args_t *boot_thread_args;

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

#define DOCK_QUEUES_TOTAL (BLOCK_QUEUES_TOTAL / 4)

typedef struct dock_thread_pool_s {
  pthread_t threads[DOCK_QUEUES_TOTAL];
  dock_queue_t queue[DOCK_QUEUES_TOTAL];
  pthread_t boot_thread;
} dock_thread_pool_t;

void dock_queue_init(dock_queue_t *dock_queue);
void dock_enqueue(block_t *block);
block_t *dock_dequeue(dock_queue_t *dock_queue);
void *dock_thread_pool_init(void *not_used);
void dock_thread_pool_delete();
void dock_thread_pool_launch();

extern dock_thread_pool_t dock_thread_pool;

#endif // DOCK_H
