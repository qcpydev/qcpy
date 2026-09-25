#include <block.h>
#include <pthread.h>
#include <qlog.h>

#pragma once
#ifndef EXPORTER_H
#define EXPORTER_H

/**
 * Port exporter manages, well, the exporting of data out of qcpy_core. This can
 * be either sending data to qcpy_connect (front end of qcpy)
 * TODO: Fix this description
 * */

void exporter_process();
export_t *exporter_init(int exporter_memfd, int exporter_migrate_memfd);
void exporter_await_quack_start();

void exporter_flush_init();
void exporter_flush_signal();
void exporter_flush();

typedef struct exporter_signal_item_t exporter_signal_item_t;
struct exporter_signal_item_t {
  exporter_signal_item_t *next;
  qlog_t *qlog;
};

exporter_signal_item_t *exporter_signal_item_init(qlog_t *qlog);
void exporter_signal_item_delete(exporter_signal_item_t *sig_item);
void exporter_signal_item_append(qlog_t *qlog);

qlog_t *exporter_signal_item_dequeue();

typedef struct exporter_signal_s {
  pthread_mutex_t lock;
  pthread_cond_t cond;
  exporter_signal_item_t *items;
  exporter_signal_item_t *last;
  uint64_t count;
} exporter_signal_t;

extern exporter_signal_t exporter_signal;
#endif
