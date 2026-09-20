#include <assert.h>
#include <exporter.h>
#include <exporter_migrate.h>
#include <port.h>
#include <pthread.h>
#include <qlog_infra.h>
#include <qlog_optimize.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

exporter_signal_t exporter_signal;

void exporter_flush_init() {
  pthread_mutex_init(&exporter_signal.lock, NULL);
  pthread_cond_init(&exporter_signal.cond, NULL);
}

export_t *exporter_init(int exporter_memfd) {
  assert(exporter_memfd != -1);

  export_t *exporter = mmap(NULL, sizeof(export_t), PROT_READ | PROT_WRITE,
                            MAP_SHARED, exporter_memfd, 0);

  assert(open != MAP_FAILED);

  assert(!atomic_load_explicit(&exporter->export_ready, memory_order_acquire));
  atomic_store_explicit(&exporter->export_ready, true, memory_order_release);

  return exporter;
}

void exporter_process() {
  qlog_t *qlog = exporter_signal_item_dequeue();

  qlog_graph_t *graph_to_process = qlog->graph;
  assert(graph_to_process);

  uint32_t size = graph_to_process->size;

  qlog_node_t **to_copy = graph_to_process->nodes;
  qlog_node_t **qlog_nodes = NULL;

  qlog_nodes = (qlog_node_t **)malloc(sizeof(qlog_node_t *) * size);
  assert(qlog_nodes);

  memcpy(qlog_nodes, to_copy, sizeof(qlog_node_t *) * size);

  exporter_migrate_fill_queue(qlog_nodes, size);
  free(qlog_nodes);

  assert(qlog);
}

exporter_signal_item_t *exporter_signal_item_init(qlog_t *qlog) {
  assert(qlog);

  exporter_signal_item_t *sig_item;

  sig_item = (exporter_signal_item_t *)malloc(sizeof(exporter_signal_item_t));
  assert(sig_item);

  memset(sig_item, 0, sizeof(exporter_signal_item_t));

  sig_item->qlog = qlog;
  return sig_item;
}

void exporter_signal_item_delete(exporter_signal_item_t *sig_item) {
  assert(sig_item);
  free(sig_item);
  sig_item = NULL;
}

qlog_t *exporter_signal_item_dequeue() {
  assert(exporter_signal.items);

  qlog_t *qlog = exporter_signal.items->qlog;

  exporter_signal_item_delete(exporter_signal.items);
  exporter_signal.count--;

  return qlog;
}

void exporter_signal_item_append(qlog_t *qlog) {
  assert(qlog);
  exporter_signal_item_t *sig_item = exporter_signal_item_init(qlog);

  assert(sig_item);

  pthread_mutex_lock(&exporter_signal.lock);
  if (!exporter_signal.items) {
    exporter_signal.items = sig_item;
    exporter_signal.last = sig_item;
  } else {
    exporter_signal.last->next = sig_item;
    exporter_signal.last = exporter_signal.last->next;
  }

  exporter_signal.count++;

  pthread_cond_signal(&exporter_signal.cond);
  pthread_mutex_unlock(&exporter_signal.lock);
}
