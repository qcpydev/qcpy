#include "qlog.h"
#include "qlog_stats/qlog_stats.h"
#include "qlog_error/qlog_error.h"
#include "qlog_optimize/qlog_optimize.h"

uint16_t max_qlog_length = UINT16_MAX - 1;
uint16_t empty_qlog_size = UINT16_MAX;
uint8_t max_str_length = UINT8_MAX;
uint16_t max_qlog_qubits = UINT16_MAX;
uint8_t matrix_unitarty_format = 2;

#define MAX_STR_LENGTH (max_str_length)
#define MAX_QLOG_QUBITS (max_qlog_qubits)
#define MATRIX_UNITARY_FORMAT (matrix_unitary_format)
#define IS_QLOG_EMPTY(qlog) (qlog->qlog_size == EMPTY_QLOG)
#define GATE_TYPE_TO_STRING (qlog_entry_gate) (gate_names[qlog_entry_gate])

struct qlog_def* qlog_init(uint8_t qubits) {
  struct qlog_def* qlog = (struct qlog_def*)malloc(sizeof(struct qlog_def));
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "malloc failed", QLOG_ERROR);
  }
  qlog->qlog_qubit_cnt = qubits;
  qlog->qlog_size = 0;
  qlog->qlog_entry_list = qlog_entry_dummy_init();
  qlog->qlog_entry_last = qlog->qlog_entry_list;
  qlog->qlog_stat = qlog_stats_init();
  qlog->qlog_optimize = qlog_optimize_init(qlog);
  return qlog;
}

void qlog_delete(struct qlog_def* qlog) {
  for (uint16_t i = qlog->qlog_size; i > 0; --i) {
    struct qlog_entry_def* temp_ptr = qlog->qlog_entry_last;
    qlog->qlog_entry_last = qlog->qlog_entry_last->qlog_entry_prev;
    qlog_entry_delete(temp_ptr);
  }
  qlog_entry_delete(qlog->qlog_entry_list);
  qlog_stats_delete(qlog->qlog_stat);
  qlog = NULL;
  return;
}

uint16_t qlog_size(struct qlog_def *qlog) {
  if (!qlog) {
    return 0;
  }
  return qlog->qlog_size;
}

void qlog_clear(struct qlog_def *qlog) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "null qlog", QLOG_WARNING);
  }

  struct qlog_entry_def* qlog_walker = qlog->qlog_entry_list->qlog_entry_next;

  while (qlog_walker) {
    qlog_entry_delete(qlog_walker);
    qlog_walker = qlog_walker->qlog_entry_next;
  }

  qlog->qlog_size = 0;
  return;
}

qlog_append_res qlog_append(struct qlog_def *qlog, uint8_t *qubits, uint8_t num_qubits, int type, int gate, float16 theta, float16 phi, float16 lambda) {
  if (!qlog) {
    return QLOG_APPEND_ERROR;
  }

  if (num_qubits > qlog->qlog_qubit_cnt) {
    QLOG_SET_ERROR(qlog, "num_qubits greater than current qlog qubit count", QLOG_ERROR);
  }

  qlog->qlog_entry_last = qlog_entry_init(qlog->qlog_size, qlog->qlog_entry_last, qubits, num_qubits, type, gate, theta, phi, lambda);

  if (!qlog->qlog_entry_last) {
    QLOG_SET_ERROR(qlog, "qlog failed to append entry", QLOG_ERROR);
  }

  qlog->qlog_size += 1;
  
  qlog_optimize_append(qlog->qlog_optimize, qlog->qlog_entry_last);

  return QLOG_APPEND_SUCCESS;
}

qlog_append_res qlog_append_entry(struct qlog_def *qlog, struct qlog_entry_def *qlog_entry) {

  if (!qlog_entry) {
    QLOG_SET_ERROR(qlog, "qlog entry is null", QLOG_ERROR);
  }
  
  float theta, phi, lambda;
  theta = phi = lambda = 0.0;
  return qlog_append(qlog,
                     qlog_entry_deconstruct_qubit_flags(qlog_entry),
                     qlog_entry->qlog_entry_qubit_cnt,
                     qlog_entry->qlog_entry_gate_type,
                     qlog_entry->qlog_entry_gate,
                     theta,
                     phi,
                     lambda);
}

void qlog_dump_content(struct qlog_def *qlog, bool verbose) {
  if (!qlog) {
    return;
  }
  if (qlog->qlog_size == 0) {
    printf("qlog is empty\n.");
  }
  if (verbose) {
    printf("qlog size: %d, number of qubits: %d\n", qlog->qlog_size, qlog->qlog_qubit_cnt);
    qlog_stats_dump_log(qlog->qlog_stat);
  } 
  struct qlog_entry_def* qlog_walker = qlog->qlog_entry_list->qlog_entry_next;
  if (!qlog_walker) {
    return;
  }

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_dump_content(qlog_walker, verbose);
    qlog_walker = qlog_walker->qlog_entry_next;
  }
  return;
}

bool qlog_compare_qlogs(struct qlog_def *qlog, struct qlog_def *qlog_to_compare) {
  if (!qlog || !qlog_to_compare) {
    return false;
  }

  if (!qlog && !qlog_to_compare) {
    return true;
  }

  if (qlog->qlog_size != qlog_to_compare->qlog_size) {
    return false;
  }

  struct qlog_entry_def* qlog_walker = qlog->qlog_entry_list->qlog_entry_next;
  struct qlog_entry_def* qlog_walker_compare = qlog_to_compare->qlog_entry_list->qlog_entry_next;

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_compare_entries(qlog_walker, qlog_walker_compare);
    qlog_walker = qlog_walker->qlog_entry_next;
    qlog_walker_compare = qlog_walker_compare->qlog_entry_next;
  }

  return true;
}

struct qlog_def* qlog_combine_qlogs(struct qlog_def* qlog, struct qlog_def* qlog_to_add) {
  if (!qlog) {
    return qlog_to_add;
  }

  if (!qlog_to_add) {
    return qlog;
  }

  if (qlog_to_add->qlog_size == 0) {
    return qlog;
  }

  qlog->qlog_size += qlog_to_add->qlog_size;
  qlog->qlog_entry_last->qlog_entry_next = qlog_to_add->qlog_entry_list->qlog_entry_next; 
  qlog->qlog_entry_last = qlog_to_add->qlog_entry_last;

  return qlog;
} 

char** qlog_get_gate_names(struct qlog_def *qlog) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog is null", QLOG_ERROR);
  }

  char** gate_names = (char**)malloc(sizeof(char*) * qlog->qlog_size * MAX_STR_LENGTH);
  
  if (!gate_names) {
    QLOG_SET_ERROR(qlog, "gate_names failed malloc", QLOG_ERROR);
  }

  struct qlog_entry_def* qlog_walker = qlog->qlog_entry_list->qlog_entry_next;
  
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    char* qlog_gate_name = (char*) get_qlog_entry_gate(qlog_walker);
    gate_names[i] = qlog_gate_name;
    qlog_walker = qlog_walker->qlog_entry_next;
  }
  return gate_names;
}

char** qlog_get_gate_types(struct qlog_def* qlog) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog is null", QLOG_ERROR);
  }
  char** gate_types = (char**)malloc(sizeof(char*) * qlog->qlog_size * MAX_STR_LENGTH);

  if (!gate_types) {
    QLOG_SET_ERROR(qlog, "gate_types failed malloc", QLOG_ERROR);
  }

  struct qlog_entry_def* qlog_walker = qlog->qlog_entry_list->qlog_entry_next;

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    gate_types[i] = (char*)get_qlog_entry_gate_type(qlog_walker);
    qlog_walker = qlog_walker->qlog_entry_next;
  }

  return gate_types;
}

uint8_t** qlog_get_gate_qubits(struct qlog_def* qlog) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog is null", QLOG_ERROR);
  }
  
  struct qlog_entry_def* qlog_walker = qlog->qlog_entry_list->qlog_entry_next;
  uint8_t** qlog_gate_qubits = (uint8_t**)malloc(sizeof(uint8_t*) * qlog->qlog_size);

  if (!qlog_gate_qubits) {
    QLOG_SET_ERROR(qlog, "qlog_gate_qubits failed malloc", QLOG_ERROR);
  }

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_gate_qubits[i] = qlog_entry_deconstruct_qubit_flags(qlog_walker);
    qlog_walker = qlog_walker->qlog_entry_next;
  }
  return qlog_gate_qubits;
}

uint8_t* qlog_get_entry_sizes(struct qlog_def* qlog) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog is null", QLOG_ERROR);
  }

  struct qlog_entry_def* qlog_walker = qlog->qlog_entry_list->qlog_entry_next;

  if (!qlog_walker) {
    QLOG_SET_ERROR(qlog, "qlog does not have a correct linked list set up", QLOG_ERROR);
  }

  uint8_t* qlog_entry_sizes = (uint8_t*)malloc(sizeof(uint8_t) * qlog->qlog_size);

  if (!qlog_entry_sizes) {
    QLOG_SET_ERROR(qlog, "qlog_entry_sizes failed malloc", QLOG_ERROR);
  }

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_sizes[i] = qlog_walker->qlog_entry_qubit_cnt;
    qlog_walker = qlog_walker->qlog_entry_next;
  }

  return qlog_entry_sizes;
}

struct qlog_graph_def* qlog_make_graph(struct qlog_def* qlog, bool optimize) {
  if (!qlog) {
    QLOG_SET_ERROR(qlog, "qlog does not exist", QLOG_ERROR);
  }

  return qlog_optimize_force(qlog->qlog_optimize);
}
