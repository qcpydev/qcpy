#include <importer.h>
#include <pthread.h>
#include <qlog_register.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#pragma once
#ifndef QLOG_THREAD_POOL_H
#define QLOG_THREAD_POOL_H

/*
 * qlog thread pool is designed to handle distribution of work by "cleaning" new
 * entries in each new batch of qlog entries. qlog thread pool execution and
 * weighting will be organized by qlog scheduler. Once when the thread has
 * cleaned the new entries, it will then pass this work onto the optimize thread
 * pool, which will then determine if the new batch of work can be well,
 * optimized, to reduce redundant work.
 * */

typedef enum {
  QLOG_PROCESS_EMPTY,
  QLOG_PROCESS_START,
  QLOG_PROCESS_APPENDING,
  QLOG_PROCESS_DONE,
  QLOG_PROCESS_CLEANED_UP,
  QLOG_PROCESS_MAX
} qlog_process_state_e;

typedef struct qlog_worker_s {
  pthread_t thread;
  pthread_cond_t cond;
  pthread_mutex_t lock;
  qlog_process_state_e state;
} qlog_worker_t;

typedef struct qlog_thread_pool_s {
  qlog_worker_t workers[IMPORT_MAX_SIZE];
  uint32_t running_workers;
} qlog_thread_pool_t;

extern qlog_thread_pool_t qlog_thread_pool;

void qlog_thread_pool_init();
void* qlog_thread_pool_worker();
void qlog_thread_pool_signal_worker(uint64_t key);
void qlog_thread_pool_await();

#endif
