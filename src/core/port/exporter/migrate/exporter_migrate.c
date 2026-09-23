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
int exporter_migrate_shared = -1;

void exporter_migrate_enqueue_single(qlog_entry_t *qlog_entry, qubit_t qubit) {
  assert(qlog_entry);
  assert(qlog_entry->qubit_bitmask & (1 << qubit));

  for (;;) {
    atomic_size_t tail = atomic_load_explicit(
        &exporter_migrate->queues[qubit].tail, memory_order_acquire);

    atomic_size_t next_idx = EXPORT_MIGRATE_QUEUE_GET_IDX(++tail);

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

void exporter_migrate_dequeue_single(qubit_t qubit,
                                     qlog_entry_t *entry_dequeue) {
  assert(qubit < QUBIT_MAX_SIZE);

  for (;;) {
    atomic_size_t head = atomic_load_explicit(
        &exporter_migrate->queues[qubit].head, memory_order_acquire);

    if (head == atomic_load_explicit(&exporter_migrate->queues[qubit].tail,
                                     memory_order_acquire)) {
      continue;
    }

    memcpy(entry_dequeue, &exporter_migrate->queues[qubit].head,
           sizeof(qlog_entry_t));

    atomic_store_explicit(&exporter_migrate->queues[qubit].head,
                          EXPORT_MIGRATE_QUEUE_GET_IDX(++head),
                          memory_order_release);
    return;
  }
}

static void exporter_migrate_queue_init(qubit_t qubit) {
  assert(qubit < QUBIT_MAX_SIZE);

  atomic_store_explicit(&exporter_migrate->queues[qubit].head, 0,
                        memory_order_relaxed);

  atomic_store_explicit(&exporter_migrate->queues[qubit].tail, 0,
                        memory_order_relaxed);
}

void exporter_migrate_init() {
  assert(!exporter_migrate);

  exporter_migrate_shared = base_tools_create_shared_mem(
      sizeof(exporter_migrate_t), EXPORTER_MIGRATE_SHARED_MEM,
      (void **)&exporter_migrate);

  for (qubit_t i = 0; i < QUBIT_MAX_SIZE; ++i) {
    exporter_migrate_queue_init(i);
  }

  assert(exporter_migrate_shared != -1);
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
      exporter_migrate_enqueue_single(qlog_nodes[i]->qlog_entry, i);
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
      // send off to shared mem
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
