#include <pthread.h>
#include <importer.h>

#pragma once
#ifndef QLOG_THREAD_POOL_H
#define QLOG_THREAD_POOL_H

/*
 * qlog thread pool is designed to handle distribution of work by "cleaning" new
 * entries in each new batch of qlog entries. qlog thread pool execution and
 * weighting will be organized by qlog scheduler. Once when the thread has cleaned
 * the new entries, it will then pass this work onto the optimize thread pool,
 * which will then determine if the new batch of work can be well, optimized, to
 * reduce redundant work.
 * */

typedef struct qlog_thread_pool_s {

} qlog_thread_pool_t;

typedef struct qlog_thread_s {

} qlog_thread_t;

void qlog_thread_pool_init();
void qlog_thread_pool_delete();
void qlog_thread_pool_await();
void qlog_thread_pool_process();

#endif
