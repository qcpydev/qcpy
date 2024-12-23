#include "qlog.h"

#define MAX_QLOG_LENGTH (5000)
#define EMPTY_QLOG (MAX_QLOG_LENGTH + 1000)
#define MAX_QLOG_QUBITS (256)
#define MATRIX_UNITARY_FORMAT (2)
#define IS_QLOG_EMPTY(qlog) (qlog->qlog_size == EMPTY_QLOG)

const char *gate_names[] = {
  [QLOG_ENTRY_GATE_IDENTITY] = "IDENTITY",
  [QLOG_ENTRY_GATE_HADAMARD] = "HADAMARD",
  [QLOG_ENTRY_GATE_PAULIX] = "PAULIX",
  [QLOG_ENTRY_GATE_PAULIY] = "PAULIY",
  [QLOG_ENTRY_GATE_PAULIZ] = "PAULIZ",
  [QLOG_ENTRY_GATE_PHASE] = "PHASE",
  [QLOG_ENTRY_GATE_S] = "S",
  [QLOG_ENTRY_GATE_SDG] = "SDG",
  [QLOG_ENTRY_GATE_T] = "T",
  [QLOG_ENTRY_GATE_TDG] = "TDG",
  [QLOG_ENTRY_GATE_RZ] = "RZ",
  [QLOG_ENTRY_GATE_RY] = "RY",
  [QLOG_ENTRY_GATE_RX] = "RX",
  [QLOG_ENTRY_GATE_SX] = "SX",
  [QLOG_ENTRY_GATE_SXDG] = "SXDG",
  [QLOG_ENTRY_GATE_U] = "U",
  [QLOG_ENTRY_GATE_CX] = "CX",
  [QLOG_ENTRY_GATE_CH] = "CH",
  [QLOG_ENTRY_GATE_CY] = "CY",
  [QLOG_ENTRY_GATE_CZ] = "CZ",
  [QLOG_ENTRY_GATE_CRX] = "CRX",
  [QLOG_ENTRY_GATE_CRY] = "CRY",
  [QLOG_ENTRY_GATE_CRZ] = "CRZ",
  [QLOG_ENTRY_GATE_CR1] = "CR1",
  [QLOG_ENTRY_GATE_CCX] = "CCX",
  [QLOG_ENTRY_GATE_QFT] = "QFT",
  [QLOG_ENTRY_GATE_RCCX] = "RCCX",
  [QLOG_ENTRY_GATE_RC3X] = "RC3X",
  [QLOG_ENTRY_GATE_SWAP] = "SWAP",
  [QLOG_ENTRY_GATE_RXX] = "RXX",
  [QLOG_ENTRY_GATE_RZZ] = "RZZ",
  [QLOG_ENTRY_GATE_CUSTOM] = "CUSTOM",
  [QLOG_ENTRY_GATE_CUSTOMCONTROLLED] = "CUSTOMCONTROLLED",
  [QLOG_ENTRY_GATE_MULTI] = "MULTI",
  [QLOG_ENTRY_GATE_CUSTOMBLOCK] = "CUSTOMBLOCK",
  [QLOG_ENTRY_GATE_CUSTOMALGORITHM] = "CUSTOMALGORITHM",
};

const char *gate_types[] = {
  [QLOG_ENTRY_TYPE_SINGLE] = "SINGLE",
  [QLOG_ENTRY_TYPE_CONTROLLED] = "CONTROLLED",
  [QLOG_ENTRY_TYPE_MULTI] = "MULTI",
  [QLOG_ENTRY_TYPE_BLOCK] = "BLOCK",
  [QLOG_ENTRY_TYPE_ALGORITHM] = "ALGORITHM",
};

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
  struct qlog_stats_def qlog_stat = {0};
  qlog->qlog_stat = qlog_stat;
  return qlog;
}

void qlog_delete(struct qlog_def* qlog) {
  // free qlog_stat
  // set qlog_stat to null;

  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_delete(qlog->qlog_entries[i]);
  }
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

qlog_append_res qlog_append(struct qlog_def *qlog, uint8_t *qubits, uint8_t num_qubits, int type, int gate) {
  if (!qlog) {
    return QLOG_APPEND_ERROR;
  }
  if (qlog->qlog_size > MAX_QLOG_LENGTH) {
    return QLOG_APPEND_FULL; 
  }
  if (qlog->qlog_size == EMPTY_QLOG) {
    qlog->qlog_size = 0;
  }
  qlog->qlog_entries[qlog->qlog_size] = qlog_entry_init(qubits, num_qubits, type, gate, qlog->qlog_size);
  if (!qlog->qlog_entries[qlog->qlog_size]) {
    return QLOG_APPEND_ERROR;
  }
  qlog->qlog_size += 1;
  return QLOG_APPEND_SUCCESS;
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
    // get stats from qlog in small table
  } 
  for (uint16_t i = 0; i < qlog->qlog_size; i++) {
    printf("Entry: %d\n", i + 1);
    qlog_entry_dump_content(qlog->qlog_entries[i], verbose);
    printf("\n");
  }
  return;
}

struct qlog_entry_def* qlog_entry_init(uint8_t *qubits, uint8_t num_qubits, int type, int gate, uint8_t qlog_qubits) {
  struct qlog_entry_def* qlog_entry = (struct qlog_entry_def*)malloc(sizeof(struct qlog_entry_def));
  if (!qlog_entry) {
    return NULL;
  }
  qlog_entry->qlog_entry_qubits = (uint8_t*)malloc(num_qubits * sizeof(uint8_t));
  if (!qlog_entry->qlog_entry_qubits) {
    return NULL;
  }
  memcpy(qlog_entry->qlog_entry_qubits, qubits, num_qubits);
  qlog_entry->qlog_entry_qubit_cnt = num_qubits;
  qlog_entry->qlog_entry_gate = gate;
  qlog_entry->qlog_entry_gate_type = type;
  // set type via function to enum, handle if it does not exist
  // set gate name via function to enum, handle if it does not exist
  // set qlog_entry stat
  return qlog_entry;
}

void qlog_entry_delete(struct qlog_entry_def *qlog_entry) {
  free(qlog_entry->qlog_entry_qubits); 
  qlog_entry->qlog_entry_qubits = NULL;
  free(qlog_entry);
  qlog_entry = NULL;
  // set stats to null
  return;
}

qlog_entry_gate qlog_entry_qg_name(struct qlog_entry_def *qlog_entry) {
  return qlog_entry->qlog_entry_gate;
}

qlog_entry_type qlog_entry_qg_type(struct qlog_entry_def *qlog_entry) {
  return qlog_entry->qlog_entry_gate_type;
}

void qlog_entry_dump_content(struct qlog_entry_def *qlog_entry, bool verbose) {
  if (!qlog_entry) {
    return;
  }
  printf("(");
  if (verbose) {
    printf("Qubit Count: %d", qlog_entry->qlog_entry_qubit_cnt);
  }
  printf(" on: [");
  for (uint8_t i = 0; i < qlog_entry->qlog_entry_qubit_cnt; ++i) {
    printf("%d", qlog_entry->qlog_entry_qubits[i]);
    if (i + 1 != qlog_entry->qlog_entry_qubit_cnt) {
      printf(",");
    }
  }
  printf("] ");
  printf("%s, %s", get_qlog_entry_gate(qlog_entry), get_qlog_entry_gate_type(qlog_entry));
  printf(")");
  return;
}

struct gate_matrix_def* gate_matrix_init(uint8_t size, complex_64 **gate_matrix_gate) {
  struct gate_matrix_def *gate_matrix;
  gate_matrix->gate_matrix_size = size * 2;
  gate_matrix->gate_matrix_gate = (complex_64**)malloc((gate_matrix->gate_matrix_size) * sizeof(complex_64));
  if (!gate_matrix->gate_matrix_gate) {
  }
  memcpy(gate_matrix->gate_matrix_gate, gate_matrix_gate, gate_matrix->gate_matrix_size * sizeof(complex_64));
  return gate_matrix;
}

void gate_matrix_delete(struct gate_matrix_def *gate_matrix) {
  free(gate_matrix->gate_matrix_gate);
  gate_matrix->gate_matrix_gate = NULL;
  free(gate_matrix);
  gate_matrix = NULL;
  return;
}

uint8_t gate_matrix_get_size(struct gate_matrix_def *gate_matrix) {
  return gate_matrix->gate_matrix_size;
}

const char* get_qlog_entry_gate(struct qlog_entry_def *qlog_entry) {
  return gate_names[qlog_entry->qlog_entry_gate];
}

const char* get_qlog_entry_gate_type(struct qlog_entry_def *qlog_entry) {
  return gate_types[qlog_entry->qlog_entry_gate_type];
}
