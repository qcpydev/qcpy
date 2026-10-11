#include <base_tools.h>
#include <block.h>
#include <exporter_migrate.h>
#include <fcntl.h>
#include <port.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

exporter_migrate_t *exporter_migrate;

void exporter_migrate_enqueue_single(exporter_migrate_t *exporter_migrate,
                                     qlog_entry_t *qlog_entry, qubit_t qubit) {

  assert(exporter_migrate);
  assert(qlog_entry);
  assert(qlog_entry->qubit_bitmask & ((bitmask_t)1 << qubit));
  for (;;) {
    atomic_size_t tail = atomic_load_explicit(
        &exporter_migrate->queues[qubit].tail, memory_order_acquire);

    atomic_size_t next_idx = EXPORT_MIGRATE_QUEUE_GET_IDX(tail + 1);

    if (next_idx == atomic_load_explicit(&exporter_migrate->queues[qubit].head,
                                         memory_order_acquire)) {
      continue;
    }
    exporter_migrate->queues[qubit].entries[next_idx] = *qlog_entry;
    atomic_store_explicit(&exporter_migrate->queues[qubit].tail, next_idx,
                          memory_order_release);
    return;
  }
}

void exporter_migrate_dequeue_single(exporter_migrate_t *exporter_migrate,
                                     qubit_t qubit,
                                     qlog_entry_t *entry_dequeue) {
  assert(qubit < QUBIT_MAX_SIZE);
  assert(exporter_migrate);

  atomic_size_t head = atomic_load_explicit(
      &exporter_migrate->queues[qubit].head, memory_order_acquire);

  if (head == atomic_load_explicit(&exporter_migrate->queues[qubit].tail,
                                   memory_order_acquire)) {
    entry_dequeue->qubit_count = 0;
    return;
  }

  *entry_dequeue = exporter_migrate->queues[qubit].entries[head];

  atomic_store_explicit(&exporter_migrate->queues[qubit].head,
                        EXPORT_MIGRATE_QUEUE_GET_IDX(head + 1),
                        memory_order_release);
  return;
}

static void exporter_migrate_queue_init(qubit_t qubit) {
  assert(qubit < QUBIT_MAX_SIZE);

  atomic_store_explicit(&exporter_migrate->queues[qubit].head, 0,
                        memory_order_relaxed);

  atomic_store_explicit(&exporter_migrate->queues[qubit].tail, 0,
                        memory_order_relaxed);
}

void exporter_migrate_init(int memfd) {
  assert(!exporter_migrate);

  exporter_migrate =
      (exporter_migrate_t *)mmap(NULL, sizeof(exporter_migrate_t),
                                 PROT_READ | PROT_WRITE, MAP_SHARED, memfd, 0);
  assert(exporter_migrate);

  for (qubit_t i = 0; i < QUBIT_MAX_SIZE; ++i) {
    exporter_migrate_queue_init(i);
  }
}

static bool exporter_migrate_single_work(qlog_node_t **qlog_nodes,
                                         uint32_t size) {
  assert(qlog_nodes);
  bool single_work = false;

  for (uint32_t i = 0; i < size; ++i) {
    if (qlog_nodes[i] && (!qlog_nodes[i]->up && !qlog_nodes[i]->down)) {
      assert(qlog_nodes[i]->qlog_entry);
      single_work = true;

      assert(qlog_nodes[i]->qlog_entry);
      exporter_migrate_enqueue_single(exporter_migrate,
                                      qlog_nodes[i]->qlog_entry, i);
      qlog_nodes[i] = qlog_nodes[i]->next;
    }
  }
  return single_work;
}

static bool exporter_migrate_multi_work(qlog_node_t **qlog_nodes,
                                        uint32_t size) {
  assert(qlog_nodes);
  bool multi_work = false;

  for (uint32_t i = 0; i < size; ++i) {
    if (qlog_nodes[i] && (qlog_nodes[i]->up || qlog_nodes[i]->down)) {
      multi_work = true;

      if (qlog_nodes[i]->up) {
        qlog_nodes[i]->up = qlog_nodes[i]->up->next;
      }

      if (qlog_nodes[i]->down) {
        qlog_nodes[i]->down = qlog_nodes[i]->down->next;
      }

      qlog_nodes[i] = qlog_nodes[i]->next;
    }
  }

  return multi_work;
}

void exporter_migrate_fill_queue(qlog_node_t **qlog_nodes, uint32_t size) {
  assert(qlog_nodes);
  bool single_work = false;
  bool multi_work = false;

  do {
    do {
      single_work = exporter_migrate_single_work(qlog_nodes, size);
    } while (single_work);

    do {
      multi_work = exporter_migrate_multi_work(qlog_nodes, size);
    } while (multi_work);
  } while (single_work || multi_work);
}
