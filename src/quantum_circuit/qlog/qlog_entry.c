#include "qlog.h"

struct qlog_entry_def* qlog_entry_init(uint8_t *qubits, uint8_t num_qubits, int type, int gate, uint8_t qlog_qubits, float16 theta, float16 phi, float16 lambda) {
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
  qlog_entry->qlog_quantum_gate = quantum_gate_get_gate(gate, theta, phi, lambda);
  qlog_entry->qlog_entry_stat = qlog_entry_stats_init();
  return qlog_entry;
}

void qlog_entry_delete(struct qlog_entry_def *qlog_entry) {
  free(qlog_entry->qlog_entry_qubits); 
  qlog_entry->qlog_entry_qubits = NULL;
  free(qlog_entry);
  qlog_entry = NULL;
  return;
}

global_gate_name qlog_entry_qg_name(struct qlog_entry_def *qlog_entry) {
  return qlog_entry->qlog_entry_gate;
}

global_gate_type qlog_entry_qg_type(struct qlog_entry_def *qlog_entry) {
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

const char* get_qlog_entry_gate(struct qlog_entry_def *qlog_entry) {
  return global_get_gate_name(qlog_entry->qlog_entry_gate);
}

const char* get_qlog_entry_gate_type(struct qlog_entry_def *qlog_entry) {
  return global_get_gate_type(qlog_entry->qlog_entry_gate_type);
}

bool qlog_entry_compare_entries(struct qlog_entry_def* qlog_entry, struct qlog_entry_def* qlog_entry_to_compare) {
  if (!qlog_entry || !qlog_entry_to_compare) {
    return false;
  }
  if (!qlog_entry && !qlog_entry_to_compare) {
    return true;
  }
  if (!(qlog_entry->qlog_entry_qubit_cnt != qlog_entry_to_compare->qlog_entry_qubit_cnt ||
      qlog_entry->qlog_entry_gate_type != qlog_entry_to_compare->qlog_entry_gate_type ||
      qlog_entry->qlog_entry_gate != qlog_entry_to_compare->qlog_entry_gate) ||
      quantum_gate_compare_params(qlog_entry->qlog_quantum_gate, qlog_entry_to_compare->qlog_quantum_gate)) {
    return false;
  }
  for (uint8_t i = 0; i < qlog_entry->qlog_entry_qubit_cnt; ++i) {
    if (qlog_entry->qlog_entry_qubits[i] != qlog_entry_to_compare->qlog_entry_qubits[i]) {
      return false;
    }
  }
  return true;
}
