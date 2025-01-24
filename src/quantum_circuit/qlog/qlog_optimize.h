#include "qlog.h"
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define QLOG_OPTIMIZE_TYPES \
  X(qlog_optimize_decomp_multi_gate) \
  X(qlog_optimize_remove_identity_gates)                       

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
  int gate_removed_cnt;
  int gate_expanded_cnt;
} qlog_optimize_def;


qlog_optimize_def* qlog_optimize_init();
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
void qlog_optimize_remove_duplicate_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_remove_redundant_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
struct qlog_def* qlog_optimize_set(struct qlog_def* qlog);
// To be imported from quantum gates:

