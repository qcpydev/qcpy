#include <exporter.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <qlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

qlog_t *qlog_init(qubit_t qubits, uint16_t reg) {
  qlog_t *qlog = NULL;

  qlog = (qlog_t *)malloc(sizeof(qlog_t));

  QCPY_ASSERT(qlog, QCPY_ERROR_QLOG, qlog, QLOG_NULL);

  memset(qlog, 0, sizeof(qlog_t));

  qlog->qubit_count = qubits;
  qlog->graph = qlog_graph_init(qlog->qubit_count);
  qlog->id = reg;

  pthread_mutex_init(&qlog->lock, NULL);

  qlog_print_init(&qlog->printer, qubits);

  return qlog;
}

void qlog_delete(qlog_t *qlog) {

  QCPY_ASSERT(!qlog, QCPY_ERROR_QLOG, qlog, QLOG_NULL);

  pthread_mutex_lock(&qlog->lock);

  pthread_mutex_unlock(&qlog->lock);

  // qlog_stats_delete(qlog->stats);
  pthread_mutex_destroy(&qlog->lock);
  free(qlog);
  qlog = NULL;
  return;
}

void qlog_clear(qlog_t *qlog) {
  QCPY_ASSERT(qlog, QCPY_ERROR_QLOG, qlog, QLOG_NULL);

  pthread_mutex_lock(&qlog->lock);
  qlog->entry_count = 0;

  pthread_mutex_unlock(&qlog->lock);

  return;
}

void qlog_append(qlog_t *qlog, block_t block) {
  assert(block.type == BLOCK_QLOG_ENTRY);

  QCPY_ASSERT(qlog, QCPY_ERROR_QLOG, qlog, QLOG_NULL);

  QCPY_ASSERT((block.qubits > qlog->qubit_count), QCPY_ERROR_QLOG, qlog,
              QLOG_BAD_SIZING);

  qlog_entry_t *qlog_entry = qlog_entry_init(qlog->entry_count, block);

  pthread_mutex_lock(&qlog->lock);
  qlog->last_entry = qlog_entry;
  qlog->qubits_used |= block.qubit_bitmask;

  QCPY_ASSERT(qlog->last_entry, QCPY_ERROR_QLOG, qlog, QLOG_BAD_ENTRY);

  ++(qlog->entry_count);
  // printf("qlog_size %p size: %lu\n", qlog, qlog->entry_count);

  qlog_graph_append(qlog->graph, qlog_entry);
  qlog_print_append(&qlog->printer, block.qubit_bitmask, block.target_bitmask,
                    block.gate);

  pthread_mutex_unlock(&qlog->lock);

  if (qlog->graph->node_count % 64 == 0 && qlog->graph->node_count != 0) {
    qlog_flush_handler(qlog, 100);
  }
}

void qlog_flush_handler(qlog_t *qlog, bitmask_t work_id) {
  assert(qlog);

  exporter_signal_item_append(qlog, work_id);
}

void qlog_dump_content(qlog_t *qlog, bool verbose) {
  // stupid ahh build system, dont remove I guess???
  //
  (void)base_decompress_qubit_bitpack(0, 0);
}

bool qlog_compare(qlog_t *qlog, qlog_t *to_compare) {
  if (!qlog || !to_compare) {
    return false;
  }

  if (!qlog && !to_compare) {
    return true;
  }

  if (qlog->entry_count != to_compare->entry_count) {
    return false;
  }

  return true;
}

qlog_t *qlog_combine(qlog_t *qlog, qlog_t *to_add) {
  if (!qlog && !to_add) {
    return NULL;
  }

  if (!qlog) {
    return to_add;
  }

  if (!to_add || !to_add->entry_count) {
    return qlog;
  }

  qlog->entry_count += to_add->entry_count;
  qlog->last_entry = to_add->last_entry;

  if (qlog->qubit_count < to_add->qubit_count) {
    qlog->qubit_count = to_add->qubit_count;
  }

  return qlog;
}
