#include "qlog_entry.h"
#include "qlog.h"

struct qlog_entry_def* qlog_entry_dummy_init() {
  struct qlog_entry_def* qlog_entry = (struct qlog_entry_def*) malloc(sizeof(struct qlog_entry_def*));
  qlog_entry->qlog_entry_prev = NULL;
  qlog_entry->qlog_entry_next = NULL;
  return qlog_entry;
}

struct qlog_entry_def* qlog_entry_init(uint64_t id, struct qlog_entry_def* qlog_entry_prev, uint8_t *qubits, uint8_t num_qubits, int type, int gate, float16 theta, float16 phi, float16 lambda) { 
  if (!qlog_entry_prev) {
    return NULL;
  }

  if (!qubits) {
    return NULL;
  }

  struct qlog_entry_def* qlog_entry = (struct qlog_entry_def*) malloc(sizeof(struct qlog_entry_def));

  qlog_entry->qlog_entry_id = id;
  qlog_entry->qlog_entry_qubit_bm = qlog_entry_init_bmap(qubits, num_qubits);

  if (!qlog_entry->qlog_entry_qubit_bm) {
    return NULL;
  }

  qlog_entry->qlog_entry_prev = qlog_entry_prev;

  qlog_entry_prev->qlog_entry_next = qlog_entry;
  
  qlog_entry->qlog_entry_gate = gate;
  qlog_entry->qlog_entry_gate_type = type;
  qlog_entry->qlog_entry_qubit_cnt = num_qubits;
  qlog_entry->qlog_entry_params = quantum_gate_params_init(gate, theta, phi, lambda);
  return qlog_entry;
}

uint64_t qlog_entry_init_bmap(uint8_t* qubits, uint8_t count) {
  uint64_t bmap = 0;

  for (uint8_t i; i < count; ++i) {
    bmap |= (1 << qubits[i]);
  }
  return bmap;
}

void qlog_entry_delete(struct qlog_entry_def *qlog_entry) {
  if (!qlog_entry) {
    return;
  }
  free(qlog_entry->qlog_entry_params);
  qlog_entry->qlog_entry_params = NULL;

  free(qlog_entry->qlog_entry_stat);
  qlog_entry->qlog_entry_stat = NULL;

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
      qlog_entry->qlog_entry_gate != qlog_entry_to_compare->qlog_entry_gate)) {
    return false;
  }

  for (uint8_t i = 0; i < qlog_entry->qlog_entry_qubit_cnt; ++i) {
    if (qlog_entry->qlog_entry_qubits[i] != qlog_entry_to_compare->qlog_entry_qubits[i]) {
      return false;
    }
  }
  return true;
}
