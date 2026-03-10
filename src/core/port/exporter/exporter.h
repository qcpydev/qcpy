#include <block.h>
#include <pthread.h>

#pragma once
#ifndef EXPORTER_H
#define EXPORTER_H

/**
 * Port exporter manages, well, the exporting of data out of qcpy_core. This can
 * be either sending data to qcpy_connect (front end of qcpy) or sending qlog
 * entries to quack to process. exporter will be signaled to do work by a binary
 * semaphore, which will then signal the exporter thread to do work. The
 * exporter thread will then: 1) await for the importer to finish 2) find the
 * qlog info from qlog infra API. given this qlog, it will then flush to
 * exporter_migrate in batches, informed by exporter_migrate when quack is
 * consuming such qlog info.
 */

void exporter_process();
void exporter_init();
void exporter_await_quack_start();

typedef struct exporter_common_s {
  pthread_mutex_t dummy_lock;
} exporter_common_t;

#endif
