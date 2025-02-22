#include "qlog.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define QLOG_OPTIMIZE_TYPES \
  X(qlog_optimize_decomp_multi_gate) \
  X(qlog_optimize_remove_identity_gates) \
  X(qlog_optimize_remove_paired_dup_gates) \
  X(qlog_optimize_remove_redundant_cnot)

#define QLOG_OPTIMIZE_EXPAND_MULTI_GATE \
  X(qlog_optimize_expand_ccx) \
  X(qlog_optimize_expand_qft) \
  X(qlog_optimize_expand_rccx) \
  X(qlog_optimize_expand_rc3x) \
  X(qlog_optimize_expand_rxx) \
  X(qlog_optimize_expand_rzz) \
  X(qlog_optimize_expand_swap) \

typedef enum {
  REMOVE_IDENTITY_GATES
} qlog_opt_track_types;

typedef struct qlog_optimize_def {
  struct qlog_optimize_stats_def* qlog_optimize_stats;
  uint8_t* qlog_optimize_qubits;
  struct qlog_optimize_circuit_def* qlog_optimize_circuit;
} qlog_optimize_def;

typedef struct qlog_optimize_circuit_def {
  bool* qlog_optimize_circuit_used_qubits;
  uint16_t* qlog_optimize_circuit_row_size;
  uint8_t qlog_optimize_circuit_size;
  struct qlog_node_def** qlog_optimize_circuit;
} qlog_optimize_circuit_def;

typedef struct qlog_node_def {
  struct qlog_entry_def* qlog_entry;
  struct qlog_node_def* qlog_node_prev;
  struct qlog_node_def* qlog_node_next;
  struct qlog_node_def* qlog_node_up;
  struct qlog_node_def* qlog_node_down;
} qlog_node_def;

struct qlog_node_def* qlog_node_init(struct qlog_entry_def* qlog_entry);
void qlog_node_delete(struct qlog_node_def* qlog_node);
void qlog_node_dump(struct qlog_node_def* qlog_node);


qlog_optimize_def* qlog_optimize_init(uint8_t size);
void qlog_optimize_delete();
void qlog_optimize_decomp_multi_gate(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_ccx(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_qft(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_rccx(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_rc3x(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_rxx(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_rzz(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_swap(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_remove_identity_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_remove_paired_dup_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_remove_redundant_cnot(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
struct qlog_def* qlog_optimize_set(struct qlog_def* qlog);

struct qlog_optimize_circuit_def* qlog_optimize_circuit_init(struct qlog_def* qlog);
void qlog_optimize_circuit_delete(struct qlog_optimize_circuit_def* qlog_optimize_circuit);
int* qlog_optimize_circuit_get_row(struct qlog_optimize_circuit_def* qlog_opt_circuit, uint8_t row);

