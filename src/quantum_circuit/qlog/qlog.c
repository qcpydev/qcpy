#include "qlog.h"

uint16_t max_qlog_length = UINT16_MAX - 1;
uint16_t empty_qlog_size = UINT16_MAX;
uint8_t max_str_length = UINT8_MAX;
uint16_t max_qlog_qubits = UINT16_MAX;
uint8_t matrix_unitarty_format = 2;

#define MAX_QLOG_LENGTH (max_qlog_length)
#define EMPTY_QLOG (empty_qlog_size)
#define MAX_STR_LENGTH (max_str_length)
#define MAX_QLOG_QUBITS (max_qlog_qubits)
#define MATRIX_UNITARY_FORMAT (matrix_unitary_format)
#define IS_QLOG_EMPTY(qlog) (qlog->qlog_size == EMPTY_QLOG)
#define GATE_TYPE_TO_STRING (qlog_entry_gate) (gate_names[qlog_entry_gate])

struct qlog_def* qlog_init(uint8_t qubits) {
  struct qlog_def* qlog = (struct qlog_def*)malloc(sizeof(struct qlog_def));
  if (qlog == NULL) {
    return NULL;
  }
  qlog->qlog_qubit_cnt = qubits;
  qlog->qlog_size = 0;
  qlog->qlog_entries = (qlog_entry_def**)malloc(MAX_QLOG_LENGTH * sizeof(qlog_entry_def*));
  if (qlog->qlog_entries == NULL) {
    free(qlog);
    return NULL;
  }
  qlog->qlog_stat = qlog_stats_init();
  return qlog;
}

void qlog_delete(struct qlog_def* qlog) {
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_delete(qlog->qlog_entries[i]);
  }
  qlog_stats_delete(qlog->qlog_stat);
  free(qlog->qlog_entries);
  qlog->qlog_entries = NULL;
  free(qlog);
  qlog = NULL;
  return;
}

uint16_t qlog_size(struct qlog_def *qlog) {
  return !qlog ? (uint16_t)EMPTY_QLOG : qlog->qlog_size;
}

void qlog_clear(struct qlog_def *qlog) {
  if (!qlog) {
    return;
  }
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_delete(qlog->qlog_entries[i]);
  }
  qlog->qlog_size = EMPTY_QLOG;
  return;
}

qlog_append_res qlog_append(struct qlog_def *qlog, uint8_t *qubits, uint8_t num_qubits, int type, int gate, float16 theta, float16 phi, float16 lambda) {
  if (!qlog) {
    return QLOG_APPEND_ERROR;
  }
  if (qlog->qlog_size > MAX_QLOG_LENGTH) {
    return QLOG_APPEND_FULL; 
  }
  if (qlog->qlog_size == EMPTY_QLOG) {
    qlog->qlog_size = 0;
  }
  printf("hereher\n");
  qlog->qlog_entries[qlog->qlog_size] = qlog_entry_init(qubits, num_qubits, type, gate, qlog->qlog_size, theta, phi, lambda);
  if (!qlog->qlog_entries[qlog->qlog_size]) {
    return QLOG_APPEND_ERROR;
  }
  qlog->qlog_size += 1;
  return QLOG_APPEND_SUCCESS;
}

qlog_append_res qlog_append_entry(struct qlog_def *qlog, struct qlog_entry_def *qlog_entry) {
  if (!qlog || !qlog_entry) {
    return QLOG_APPEND_ERROR;
  }
  
  struct quantum_gate_params_def *qg_params; 
  float theta, phi, lambda = 0.0;
  if (qlog_entry->qlog_quantum_gate && qlog_entry->qlog_quantum_gate->quantum_gate_params) {
    qg_params = qlog_entry->qlog_quantum_gate->quantum_gate_params;
    theta = qg_params->theta;
    phi = qg_params->phi;
    lambda = qg_params->lambda;
  }
  return qlog_append(qlog, qlog_entry->qlog_entry_qubits,
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
  if (IS_QLOG_EMPTY(qlog)) {
    printf("qlog is empty\n.");
  }
  if (verbose) {
    printf("qlog size: %d, number of qubits: %d\n", qlog->qlog_size, qlog->qlog_qubit_cnt);
    qlog_stats_dump_log(qlog->qlog_stat);
  } 
  for (uint16_t i = 0; i < qlog->qlog_size; i++) {
    printf("Entry: %d\n", i + 1);
    qlog_entry_dump_content(qlog->qlog_entries[i], verbose);
    printf("\n");
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
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    if (!qlog_entry_compare_entries(qlog->qlog_entries[i], qlog_to_compare->qlog_entries[i])) {
      return false;
    } 
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
  if ((uint32_t)qlog->qlog_size + (uint32_t)qlog_to_add->qlog_size >= MAX_QLOG_LENGTH) {
    return qlog;
  }
  qlog->qlog_size += qlog_to_add->qlog_size;
  for (uint16_t i = 0; i < qlog_to_add->qlog_size; ++i) {
    struct qlog_entry_def* entry = qlog_to_add->qlog_entries[i];
    qlog_append_entry(qlog, qlog_to_add->qlog_entries[i]);
  }
  return qlog;
} 

char** qlog_get_gate_names(struct qlog_def *qlog) {
  if (!qlog) {
    return NULL;
  }
  char** gate_names = (char**)malloc(sizeof(char*) * qlog->qlog_size * MAX_STR_LENGTH);
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    gate_names[i] = (char*)get_qlog_entry_gate(qlog->qlog_entries[i]);
  }
  return gate_names;
}

char** qlog_get_gate_types(struct qlog_def* qlog) {
  if (!qlog) {
    return NULL;
  }
  char** gate_types = (char**)malloc(sizeof(char*) * qlog->qlog_size * MAX_STR_LENGTH);
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    gate_types[i] = (char*)get_qlog_entry_gate_type(qlog->qlog_entries[i]);
  }
  return gate_types;
}

uint8_t** qlog_get_gate_qubits(struct qlog_def* qlog) {
  if (!qlog) {
    return NULL;
  }
  uint8_t** qlog_gate_qubits = (uint8_t**)malloc(sizeof(uint8_t*) * qlog->qlog_size);
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_gate_qubits[i] = (uint8_t*)malloc(sizeof(uint8_t) * qlog->qlog_entries[i]->qlog_entry_qubit_cnt);
    memcpy(qlog_gate_qubits[i], qlog->qlog_entries[i]->qlog_entry_qubits, qlog->qlog_entries[i]->qlog_entry_qubit_cnt);
  }
  return qlog_gate_qubits;
}

uint8_t* qlog_get_entry_sizes(struct qlog_def* qlog) {
  if (!qlog) {
    return NULL;
  }
  uint8_t* qlog_entry_sizes = (uint8_t*)malloc(sizeof(uint8_t) * qlog->qlog_size);
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_sizes[i] = qlog->qlog_entries[i]->qlog_entry_qubit_cnt;
  }
  return qlog_entry_sizes;
}

