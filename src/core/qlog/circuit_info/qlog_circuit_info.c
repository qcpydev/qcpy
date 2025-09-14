#include "../qlog.h"

char **qlog_get_gate_types(struct qlog_def *qlog) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog is null", QLOG_ERROR);
  }
  char **gate_types =
      (char **)malloc(sizeof(char *) * qlog->qlog_size * MAX_STR_LENGTH);

  if (!gate_types) {
    QLOG_SET_ERROR(qlog, "gate_types failed malloc", QLOG_ERROR);
  }

  struct qlog_entry_def *qlog_walker = qlog->qlog_entry_list->qlog_entry_next;

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    gate_types[i] = (char *)get_qlog_entry_gate_type(qlog_walker);
    qlog_walker = qlog_walker->qlog_entry_next;
  }

  return gate_types;
}

uint8_t **qlog_get_gate_qubits(struct qlog_def *qlog) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog is null", QLOG_ERROR);
  }

  struct qlog_entry_def *qlog_walker = qlog->qlog_entry_list->qlog_entry_next;
  uint8_t **qlog_gate_qubits =
      (uint8_t **)malloc(sizeof(uint8_t *) * qlog->qlog_size);

  if (!qlog_gate_qubits) {
    QLOG_SET_ERROR(qlog, "qlog_gate_qubits failed malloc", QLOG_ERROR);
  }

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_gate_qubits[i] = qlog_entry_deconstruct_qubit_flags(qlog_walker);
    qlog_walker = qlog_walker->qlog_entry_next;
  }
  return qlog_gate_qubits;
}

uint8_t *qlog_get_entry_sizes(struct qlog_def *qlog) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog is null", QLOG_ERROR);
  }

  struct qlog_entry_def *qlog_walker = qlog->qlog_entry_list->qlog_entry_next;

  if (!qlog_walker) {
    QLOG_SET_ERROR(qlog, "qlog does not have a correct linked list set up",
                   QLOG_ERROR);
  }

  uint8_t *qlog_entry_sizes =
      (uint8_t *)malloc(sizeof(uint8_t) * qlog->qlog_size);

  if (!qlog_entry_sizes) {
    QLOG_SET_ERROR(qlog, "qlog_entry_sizes failed malloc", QLOG_ERROR);
  }

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_sizes[i] = qlog_walker->qlog_entry_qubit_cnt;
    qlog_walker = qlog_walker->qlog_entry_next;
  }

  return qlog_entry_sizes;
}

struct qlog_graph_def *qlog_make_graph(struct qlog_def *qlog, bool optimize) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog does not exist", QLOG_ERROR);
  }

  return qlog_optimize_force(qlog->qlog_optimize);
}
