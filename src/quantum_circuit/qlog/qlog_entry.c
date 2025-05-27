#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "qlog_entry.h"
#include "qlog.h"
#include "qlog_error/qlog_error.h"

struct qlog_entry_def* qlog_entry_dummy_init() {
  struct qlog_entry_def* qlog_entry = (struct qlog_entry_def*) malloc(sizeof(struct qlog_entry_def*));
  return qlog_entry;
}

struct qlog_entry_def* qlog_entry_init(uint64_t id,
                                       struct qlog_entry_def* qlog_entry_prev,
                                       uint8_t *qubits,
                                       uint8_t num_qubits,
                                       int type,
                                       int gate,
                                       float16 theta,
                                       float16 phi,
                                       float16 lambda) { 
  if (!qlog_entry_prev) {
    QLOG_ENTRY_SET_ERROR(qlog_entry_prev, "qlog_entry_prev is null", QLOG_ERROR);
  }

  if (!qubits) {
    QLOG_ENTRY_SET_ERROR(qlog_entry_prev, "qubits are null", QLOG_ERROR);
  }

  struct qlog_entry_def* qlog_entry = (struct qlog_entry_def*) malloc(sizeof(struct qlog_entry_def));

  qlog_entry->qlog_entry_id = id;
  qlog_entry->qlog_entry_qubits = qlog_entry_init_qubit_flags(qubits, num_qubits);

  if (!qlog_entry->qlog_entry_qubits) {
    QLOG_ENTRY_SET_ERROR(qlog_entry, "qlog entry does not have qubits", QLOG_ERROR);
  }

  qlog_entry->qlog_entry_prev = qlog_entry_prev;
  qlog_entry_prev->qlog_entry_next = qlog_entry;
  
  qlog_entry->qlog_entry_gate = gate;
  qlog_entry->qlog_entry_gate_type = type;
  qlog_entry->qlog_entry_qubit_cnt = num_qubits;
  qlog_entry->qlog_entry_params = quantum_gate_params_init(gate, theta, phi, lambda);
  qlog_entry->qlog_entry_qubit_invert = qubits[0] > qubits[num_qubits - 1];

  return qlog_entry;
}

uint64_t qlog_entry_init_qubit_flags(uint8_t* qubits, uint8_t count) {
  uint64_t qubit_int = 0;

  for (uint8_t i; i < count; ++i) {
    qubit_int |= (1 << qubits[i]);
  }

  return qubit_int;
}

uint8_t* qlog_entry_deconstruct_qubit_flags(struct qlog_entry_def* qlog_entry) {
  if (!qlog_entry) {
    QLOG_ENTRY_SET_ERROR(qlog_entry, "qlog entry is null", QLOG_ERROR); 
  }

  if (!qlog_entry->qlog_entry_qubits) {
    QLOG_ENTRY_SET_ERROR(qlog_entry, "no qlog_entry_qubits found", QLOG_ERROR);
  }

  uint8_t* qubit_decon = (uint8_t*)malloc(sizeof(uint8_t) * qlog_entry->qlog_entry_qubit_cnt);

  if (!qubit_decon) {
    QLOG_ENTRY_SET_ERROR(qlog_entry, "qubit_decon malloc failed", QLOG_ERROR);
  }

  uint64_t qubit = qlog_entry->qlog_entry_qubit_invert ? 1ULL << 62 : 1;
  uint16_t i = 0;

  do {
    if (qubit & qlog_entry->qlog_entry_qubits) {
      qubit_decon[i] = (uint8_t)log2(qubit);
      ++i;
    }

    if (qlog_entry->qlog_entry_qubit_invert) {
      qubit = qubit >> 1;
    }
    else {
      qubit = qubit << 1;

    }

  } while (qubit > 0 && qubit <= 1ULL << 63);
  
  return qubit_decon; 
}

void qlog_entry_delete(struct qlog_entry_def *qlog_entry) {
  if (!qlog_entry) {
    QLOG_ENTRY_SET_ERROR(qlog_entry, "qlog entry is null", QLOG_ERROR);
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
    QLOG_ENTRY_SET_ERROR(qlog_entry, "qlog entry is null", QLOG_ERROR);
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

struct qlog_entry_def* qlog_entry_clean_duplicate_chains(struct qlog_entry_def* qlog_entry) {
  if (!qlog_entry || qlog_entry->qlog_entry_prev) {
    return NULL;
  }

  if (!qlog_entry_compare_entries(qlog_entry, qlog_entry->qlog_entry_prev)) {
    return qlog_entry;
  }
  
  uint64_t qlog_entry_count = 1;
  struct qlog_entry_def* qlog_entry_partition = qlog_entry->qlog_entry_prev; 
  while (qlog_entry_partition->qlog_entry_prev &&
         qlog_entry_compare_entries(qlog_entry_partition, qlog_entry_partition->qlog_entry_prev)) {
    ++qlog_entry_count;
    qlog_entry_partition = qlog_entry_partition->qlog_entry_prev;
  }

  if (qlog_entry_count & 1) {
    qlog_entry_def* qlog_entry_next = qlog_entry->qlog_entry_next;
    qlog_entry_partition->qlog_entry_next->qlog_entry_next->qlog_entry_prev = qlog_entry_partition;
    qlog_entry_partition->qlog_entry_next = qlog_entry_partition->qlog_entry_next->qlog_entry_next;
    qlog_entry_delete(qlog_entry_next);
  }

  return qlog_entry_partition;
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

  return qlog_entry->qlog_entry_qubit_cnt == qlog_entry_to_compare->qlog_entry_qubit_cnt &&
      qlog_entry->qlog_entry_gate_type == qlog_entry_to_compare->qlog_entry_gate_type &&
      qlog_entry->qlog_entry_gate == qlog_entry_to_compare->qlog_entry_gate && 
      qlog_entry->qlog_entry_qubits == qlog_entry_to_compare->qlog_entry_qubits;
}
