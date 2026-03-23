#include <block.h>
#include <pthread.h>

#pragma once
#ifndef EXPORTER_H
#define EXPORTER_H

/**
 * Port exporter manages, well, the exporting of data out of qcpy_core. This can
 * be either sending data to qcpy_connect (front end of qcpy)
 * TODO: Fix this description
 * */

void exporter_process();
void exporter_init();
void exporter_await_quack_start();

void exporter_flush_init();
void exporter_flush_signal();
void exporter_flush();

typedef struct exporter_signal_item_t exporter_signal_item_t;
struct exporter_signal_item_t {
  exporter_signal_item_t *next;
  uint32_t reg;
};

exporter_signal_item_t *exporter_signal_item_init(int flush_reg);
void exporter_signal_item_delete(exporter_signal_item_t *sig_item);
void exporter_signal_item_append(int flush_reg);

int exporter_signal_item_dequeue();

typedef struct exporter_signal_s {
  pthread_mutex_t lock;
  pthread_cond_t cond;
  exporter_signal_item_t *items;
  exporter_signal_item_t *last;
} exporter_signal_t;

extern exporter_signal_t exporter_signal;
#endif
