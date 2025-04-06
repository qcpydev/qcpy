#include "qlog_optimize_graph.h"

#define EXPAND_QLOG_ENTRY(qe_gate_type) (qe_gate_type == GLOBAL_GATE_CCX || \
                                         qe_gate_type == GLOBAL_GATE_QFT || \
                                         qe_gate_type == GLOBAL_GATE_RCCX ||\
                                         qe_gate_type == GLOBAL_GATE_RXX || \
                                         qe_gate_type == GLOBAL_GATE_RZZ || \
                                         qe_gate_type == GLOBAL_GATE_SWAP ||\
                                         qe_gate_type == GLOBAL_GATE_RC3X)


void (*qlog_optimize_types[])(struct qlog_def* qlog, struct qlog_def* qlog_optimized, struct qlog_optimize_def* qlog_optimize) = {
  qlog_optimize_remove_identity_gates,
  qlog_optimize_remove_paired_dup_gates
};

uint8_t qlog_optimize_types_size = sizeof(qlog_optimize_types) / sizeof(qlog_optimize_types[0]);

void (*qlog_optimize_expand_types[])(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry) = {
  [GLOBAL_GATE_CCX] = qlog_optimize_expand_ccx,
  [GLOBAL_GATE_QFT] = qlog_optimize_expand_qft,  
  [GLOBAL_GATE_RCCX] = qlog_optimize_expand_rccx,
  [GLOBAL_GATE_RC3X] = qlog_optimize_expand_rc3x,
  [GLOBAL_GATE_RXX] = qlog_optimize_expand_rxx,
  [GLOBAL_GATE_RZZ] = qlog_optimize_expand_rzz,
  [GLOBAL_GATE_SWAP] = qlog_optimize_expand_swap
};

struct qlog_optimize_def* qlog_optimize_init(struct qlog_def* qlog) {
  if (!qlog) {
    return NULL;
  }
  struct qlog_optimize_def* qlog_optimize = (struct qlog_optimize_def*) malloc(sizeof(struct qlog_optimize_def));
  qlog_optimize->qlog_optimize_stats = qlog_optimize_stats_init(); 
  qlog_optimize->qlog_optimize_qubits = (uint8_t*)malloc(sizeof(uint8_t) * qlog->qlog_size);
  qlog_optimize->qlog_optimize_size = qlog->qlog_size;
  qlog_optimize->qlog_optimize_qlog_ptr = qlog;
  return qlog_optimize;
}

struct qlog_def* qlog_optimize_set(struct qlog_def *qlog) {
  struct qlog_def* qlog_expanded = qlog_optimize_expand_multi_gate(qlog);
  struct qlog_def* optimized_qlog;
  struct qlog_optimize_def* qlog_optimize;
  for (int i = 0; i < qlog_optimize_types_size; ++i) {
    optimized_qlog = qlog_init(qlog_expanded->qlog_qubit_cnt); 
    qlog_optimize = qlog_optimize_init(qlog_expanded);
    qlog_optimize_types[i](qlog_expanded, optimized_qlog, qlog_optimize);
    memcpy(qlog_expanded, optimized_qlog, sizeof(struct qlog_def));
  }
  return optimized_qlog;
}

struct qlog_def* qlog_optimize_expand_multi_gate(struct qlog_def* qlog){ 
  struct qlog_def* expand_qlog = qlog_init(qlog->qlog_qubit_cnt);
  for (int i = 0; i < qlog->qlog_size; ++i) {
    

  }
  return expand_qlog;
}

void qlog_optimize_expand_ccx(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry) {
  qlog_append_res append_res;    
  uint8_t control_one = qlog_entry->qlog_entry_qubits[0];
  uint8_t control_two = qlog_entry->qlog_entry_qubits[1];
  uint8_t target = qlog_entry->qlog_entry_qubits[2];
  append_res = qlog_append(qlog, (uint8_t[]){control_two, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){control_one, control_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){control_two, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSXDG, 0 ,0 ,0);
  append_res = qlog_append(qlog, (uint8_t[]){control_one, control_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0 ,0 ,0);
  append_res = qlog_append(qlog, (uint8_t[]){control_one, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
}

void qlog_optimize_expand_qft(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry) {
  qlog_append_res append_res;
  uint8_t qubit_one = qlog_entry->qlog_entry_qubits[0]; 
  uint8_t qubit_two = qlog_entry->qlog_entry_qubits[1]; 
  uint8_t qubit_three = qlog_entry->qlog_entry_qubits[2]; 
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one}, 2, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_HADAMARD, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_two, qubit_one}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CR1, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_two, qubit_three}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CR1, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_two}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_HADAMARD, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_three, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CR1, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_three}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_HADAMARD, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one, qubit_three}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_three, qubit_one}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one, qubit_three}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
}

  void qlog_optimize_expand_rccx(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry) {
    qlog_append_res append_res;
    uint8_t control_one = qlog_entry->qlog_entry_qubits[0];
    uint8_t control_two = qlog_entry->qlog_entry_qubits[1];
    uint8_t target = qlog_entry->qlog_entry_qubits[2];
    append_res = qlog_append(qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
    append_res = qlog_append(qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
    append_res = qlog_append(qlog, (uint8_t[]){control_two, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
    append_res = qlog_append(qlog, (uint8_t[]){control_one, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
    append_res = qlog_append(qlog, (uint8_t[]){control_two, target}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CSX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){target}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, 0);
  }

  void qlog_optimize_expand_rc3x(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry) {
    qlog_append_res append_res;
    uint8_t qubit_1 = qlog_entry->qlog_entry_qubits[0];
    uint8_t qubit_2 = qlog_entry->qlog_entry_qubits[1];
    uint8_t qubit_3 = qlog_entry->qlog_entry_qubits[2];
    uint8_t qubit_4 = qlog_entry->qlog_entry_qubits[3];
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_3, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_1, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_2, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_1, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_2, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, M_PI / 4);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_3, qubit_4}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, 0, (-1 * M_PI) / 4);
    append_res = qlog_append(qlog, (uint8_t[]){qubit_4}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, 0, M_PI);
  }

  void qlog_optimize_expand_rxx(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry) {
    qlog_append_res append_res;
    uint8_t qubit_one = qlog_entry->qlog_entry_qubits[0];
    uint8_t qubit_two = qlog_entry->qlog_entry_qubits[1];
    float theta = qlog_entry->qlog_entry_params->theta;
  append_res = qlog_append(qlog, (uint8_t[]){qubit_two}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_HADAMARD, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_two}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, -1 * theta, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, M_PI / 2, -1 * M_PI, M_PI - theta);
}

void qlog_optimize_expand_rzz(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry) {
  qlog_append_res append_res;
  uint8_t qubit_one = qlog_entry->qlog_entry_qubits[0];
  uint8_t qubit_two = qlog_entry->qlog_entry_qubits[1];
  float theta = qlog_entry->qlog_entry_params->theta;
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one}, 1, GLOBAL_TYPE_SINGLE, GLOBAL_GATE_U, 0, theta, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one, qubit_two}, 1, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
}

void qlog_optimize_expand_swap(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry) {
  qlog_append_res append_res;
  uint8_t qubit_one = qlog_entry->qlog_entry_qubits[0];
  uint8_t qubit_two = qlog_entry->qlog_entry_qubits[1];
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_two, qubit_one}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
  append_res = qlog_append(qlog, (uint8_t[]){qubit_one, qubit_two}, 2, GLOBAL_TYPE_CONTROLLED, GLOBAL_GATE_CX, 0, 0, 0);
}

void qlog_optimize_remove_identity_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, struct qlog_optimize_def* qlog_optimize) {
}

void qlog_optimize_remove_paired_dup_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, struct qlog_optimize_def* qlog_optimize) {
  struct qlog_graph_def* qlog_graph = qlog_graph_init(qlog_optimize);
  if (!qlog_graph || !qlog_graph->qlog_graph_circuit_track) {
    return;
  }
  
  //qlog_graph_serialize(qlog_graph, optimized_qlog);
}

void qlog_optimize_remove_redundant_cnot(struct qlog_def* qlog, struct qlog_def* optimize_qlog, struct qlog_optimize_def* qlog_optimize) {
}

