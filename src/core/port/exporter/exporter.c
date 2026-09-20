#include <assert.h>
#include <exporter.h>
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

// TODO: MOVE THIS TO QUACK_EXPORT. We will never actually need to use this
// shared mem, so whats the point of opening it! We will use our own shared mem
// for exporter to send info to quack. We are just the mediator for sending over
// stuff to quack
//

void exporter_flush_init() { pthread_mutex_init(&exporter_signal.lock, NULL); }

void exporter_init() {}

void exporter_process() {
  int to_process = exporter_signal_item_dequeue();

  qlog_t *qlog = qlog_infra_find_qlog(to_process);
  pthread_mutex_lock(&qlog->lock);

  assert(qlog);

  qlog_graph_t *graph_to_process = qlog->graph;
  assert(graph_to_process);

  uint32_t size = graph_to_process->size;

  qlog_node_t **to_copy = graph_to_process->nodes;
  qlog_node_t **qlog_nodes = NULL;

  qlog_nodes = (qlog_node_t **)malloc(sizeof(qlog_node_t *) * size);
  assert(qlog_nodes);

  memcpy(qlog_nodes, to_copy, sizeof(qlog_node_t *) * size);

  // exporter_migrate_fill_queue(qlog_nodes, size);
  free(qlog_nodes);

  pthread_mutex_unlock(&qlog->lock);
}

exporter_signal_item_t *exporter_signal_item_init(int flush_reg) {
  exporter_signal_item_t *sig_item;

  sig_item = (exporter_signal_item_t *)malloc(sizeof(exporter_signal_item_t));
  assert(sig_item);

  memset(sig_item, 0, sizeof(exporter_signal_item_t));

  sig_item->reg = flush_reg;
  return sig_item;
}

void exporter_signal_item_delete(exporter_signal_item_t *sig_item) {
  assert(sig_item);
  free(sig_item);
  sig_item = NULL;
}

int exporter_signal_item_dequeue() {
  assert(exporter_signal.items);
  int reg = exporter_signal.items->reg;

  exporter_signal_item_t *sig_item = exporter_signal.items;
  bool was_last = sig_item == exporter_signal.last;

  if (exporter_signal.items->next) {
    exporter_signal.items = exporter_signal.items->next;
  }

  if (was_last) {
    exporter_signal.items = NULL;
    exporter_signal.last = NULL;
  }

  exporter_signal_item_delete(sig_item);

  return reg;
}

void exporter_signal_item_append(int flush_reg) {
  exporter_signal_item_t *sig_item = exporter_signal_item_init(flush_reg);

  assert(sig_item);

  if (!exporter_signal.items) {
    exporter_signal.items = sig_item;
    exporter_signal.last = sig_item;
  } else {
    exporter_signal.last->next = sig_item;
    exporter_signal.last = exporter_signal.last->next;
  }
}
