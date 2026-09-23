#include <block.h>
#include <qlog.h>
#include <qlog_graph.h>
#include <stdatomic.h>
#include <stdint.h>

#pragma once
#ifndef EXPORTER_MIGRATE_H
#define EXPORTER_MIGRATE_H

/*
 * Slightly "Pun intended" subcomponent of exporter to migrate work to send to
 * quack (ducks migrate...and they quack too), regardless of cuda enabled or
 * not. This shared memory space should not be accessed by qcpy front end, which
 * allows the connection between the qcpy_core and quack_core/_gpu. We expect a
 * stream of a qlog's information to be sent to this shared memory space for
 * quack to consume.
 */

#define EXPORTER_MIGRATE_SHARED_MEM "/qcpy_exporter_migrate_shared_mem"

#define EXPORTER_MIGRATE_QUBIT_QUEUE_SIZE 32u

#define EXPORT_MIGRATE_QUEUE_MAX_IDX (EXPORTER_MIGRATE_QUBIT_QUEUE_SIZE - 1)
#define EXPORT_MIGRATE_QUEUE_GET_IDX(idx) (idx & EXPORT_MIGRATE_QUEUE_MAX_IDX)

typedef struct exporter_migrate_queue_s {
  atomic_size_t head;
  dont_use_cache_align_t _head[CACHE_LINE_SIZE - sizeof(atomic_size_t)];

  atomic_size_t tail;
  dont_use_cache_align_t _tail[CACHE_LINE_SIZE - sizeof(atomic_size_t)];

  qlog_entry_t entries[EXPORTER_MIGRATE_QUBIT_QUEUE_SIZE];
} exporter_migrate_queue_t;

typedef struct exporter_migrate_s {
  exporter_migrate_queue_t queues[QUBIT_MAX_SIZE];
} exporter_migrate_t;

extern exporter_migrate_t *exporter_migrate;
extern sem_t *exporter_migrate_ready;
extern sem_t *exporter_migrate_empty;
extern int exporter_migrate_shared;

void exporter_migrate_dequeue(qlog_entry_t *qlog_entry);
void exporter_migrate_enqueue_single(qlog_entry_t *qlog_entry, qubit_t qubit);
void exporter_migrate_init();
void exporter_migrate_fill_queue(qlog_node_t **qlog_nodes, uint32_t size);

#endif
