#include "qlog_optimize.h"
#include <stdint.h>

#define X(f) f,
void (*qlog_optimize_types_arr[])(struct qlog_def*, struct qlog_def*, qlog_optimize_def*) = {QLOG_OPTIMIZE_TYPES};
void (*qlog_optimize_expand_types_arr[])(struct qlog_def*, struct qlog_def*, struct qlog_optimize_def*) = {QLOG_OPTIMIZE_EXPAND_MULTI_GATE};
#undef X
int qlog_optimize_types_arr_cnt = sizeof(qlog_optimize_types_arr) / sizeof(qlog_optimize_types_arr[0]);
int qlog_optimize_expand_types_arr_cnt = sizeof(qlog_optimize_expand_types_arr) / sizeof(qlog_optimize_types_arr[0]);

struct qlog_optimize_def* qlog_optimize_init(struct qlog_def* qlog) {
  struct qlog_optimize_def* qlog_optimize;
  qlog_optimize->qlog_optimize_stats = qlog_optimize_stats_init(); 
  qlog_optimize->qlog_optimize_qubits = (uint8_t*)malloc(sizeof(uint8_t) * qlog->qlog_size);
  qlog_optimize->qlog_optimize_size = qlog->qlog_size;
  qlog_optimize->qlog_optimize_qlog_ptr = qlog;
  return qlog_optimize;
}

struct qlog_def* qlog_optimize_set(struct qlog_def *qlog) {
  struct qlog_def* optimized_qlog = qlog_init(qlog->qlog_qubit_cnt); 
  struct qlog_optimize_def* qlog_optimize = qlog_optimize_init(qlog);
  for (int i = 0; i < qlog_optimize_types_arr_cnt; ++i) {
    qlog_optimize_types_arr[i](qlog, optimized_qlog, qlog_optimize);
  }
  return optimized_qlog;
}

void qlog_optimize_decomp_multi_gate(struct qlog_def* qlog, struct qlog_def* optimized_qlog, struct qlog_optimize_def* qlog_optimize) { 
  for (int i = 0; i < qlog_optimize_expand_types_arr_cnt; ++i) {
    qlog_optimize_expand_types_arr[i](qlog, optimized_qlog, qlog_optimize);
  }
}

void qlog_optimize_expand_ccx(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize) {
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
    qlog_append_res append_res;
    if (qlog_entry->qlog_entry_gate == GLOBAL_GATE_CCX) {
      uint8_t control_one = qlog_entry->qlog_entry_qubits[0];
      uint8_t control_two = qlog_entry->qlog_entry_qubits[1];
      uint8_t target = qlog_entry->qlog_entry_qubits[2];
      append_res = qlog_append(optimized_qlog, (uint8_t[]){control_two, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){control_one, control_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){control_two, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSXDG, 0 ,0 ,0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){control_one, control_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0 ,0 ,0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){control_one, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
    }
    else {
      append_res = qlog_append_entry(optimized_qlog, qlog->qlog_entries[i]);
    }
  }
}

void qlog_optimize_expand_qft(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize) {
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
    qlog_append_res append_res;
    if (qlog_entry->qlog_entry_gate == GLOBAL_GATE_QFT) {
      uint8_t qubit_one = qlog_entry->qlog_entry_qubits[0]; 
      uint8_t qubit_two = qlog_entry->qlog_entry_qubits[1]; 
      uint8_t qubit_three = qlog_entry->qlog_entry_qubits[2]; 
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one}, 2, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_HADAMARD, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_two, qubit_one}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CR1, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_two, qubit_three}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CR1, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_two}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_HADAMARD, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_three, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CR1, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_three}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_HADAMARD, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one, qubit_three}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_three, qubit_one}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one, qubit_three}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
    }
    else {
      append_res = qlog_append_entry(optimized_qlog, qlog->qlog_entries[i]);
    }
  }
}

void qlog_optimize_expand_rccx(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize) {
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
    qlog_append_res append_res;
    if (qlog_entry->qlog_entry_gate == GLOBAL_GATE_RCCX) {
      uint8_t control_one = qlog_entry->qlog_entry_qubits[0];
      uint8_t control_two = qlog_entry->qlog_entry_qubits[1];
      uint8_t target = qlog_entry->qlog_entry_qubits[2];
      append_res = qlog_append(optimized_qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){control_two, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){control_one, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){control_two, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, 0);
    }
    else {
      append_res = qlog_append_entry(optimized_qlog, qlog->qlog_entries[i]);
    }
  }
}

void qlog_optimize_expand_rc3x(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize) {
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
    qlog_append_res append_res;
    if (qlog_entry->qlog_entry_gate == GLOBAL_GATE_RC3X) { 
      uint8_t qubit_1 = qlog_entry->qlog_entry_qubits[0];
      uint8_t qubit_2 = qlog_entry->qlog_entry_qubits[1];
      uint8_t qubit_3 = qlog_entry->qlog_entry_qubits[2];
      uint8_t qubit_4 = qlog_entry->qlog_entry_qubits[3];
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_3, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_1, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_2, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_1, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_2, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_3, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
    }
    else {
      append_res = qlog_append_entry(optimized_qlog, qlog->qlog_entries[i]);
    }
  }
}

void qlog_optimize_expand_rxx(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize) {
  for (uint16_t i = 0; i  < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
    qlog_append_res append_res;
    if (qlog_entry->qlog_entry_gate == GLOBAL_GATE_RXX) {
      uint8_t qubit_one = qlog_entry->qlog_entry_qubits[0];
      uint8_t qubit_two = qlog_entry->qlog_entry_qubits[1];
      float theta = qlog_entry->qlog_quantum_gate->quantum_gate_params->theta;
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_two}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_HADAMARD, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_two}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, -1 * theta, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, -1 * M_PI, M_PI - theta);
    }
    else {
      append_res = qlog_append_entry(optimized_qlog, qlog->qlog_entries[i]);
    }
  }
}

void qlog_optimize_expand_rzz(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize) {
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
    qlog_append_res append_res;
    if (qlog_entry->qlog_entry_gate == GLOBAL_GATE_RZZ) {
      uint8_t qubit_one = qlog_entry->qlog_entry_qubits[0];
      uint8_t qubit_two = qlog_entry->qlog_entry_qubits[1];
      float theta = qlog_entry->qlog_quantum_gate->quantum_gate_params->theta;
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, theta, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one, qubit_two}, 1, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
    }
    else {
      append_res = qlog_append_entry(optimized_qlog, qlog->qlog_entries[i]);
    }
  }
}

void qlog_optimize_expand_swap(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize) {
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
    qlog_append_res append_res;
    if (qlog_entry->qlog_entry_gate == GLOBAL_GATE_SWAP) {
      uint8_t qubit_one = qlog_entry->qlog_entry_qubits[0];
      uint8_t qubit_two = qlog_entry->qlog_entry_qubits[1];
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_two, qubit_one}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
      append_res = qlog_append(optimized_qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
    }
    else {
      append_res = qlog_append_entry(optimized_qlog, qlog->qlog_entries[i]);
    }
  }
}

void qlog_optimize_remove_identity_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, struct qlog_optimize_def* qlog_optimize) {
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
    if (qlog_entry->qlog_entry_gate != GLOBAL_GATE_IDENTITY) {
      qlog_append_res append_res = qlog_append_entry(optimized_qlog, qlog->qlog_entries[i]);
    }
  }
}

void qlog_optimize_remove_paired_dup_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, struct qlog_optimize_def* qlog_optimize) {
  
  for (uint16_t i = 0; i < qlog->qlog_size; ++i) {
    qlog_entry_def* qlog_entry = qlog->qlog_entries[i];
  }
}

void qlog_optimize_remove_redundant_cnot(struct qlog_def* qlog, struct qlog_def* optimize_qlog, struct qlog_optimize_def* qlog_optimize) {

}

