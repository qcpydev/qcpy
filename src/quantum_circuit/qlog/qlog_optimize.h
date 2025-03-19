#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "qlog.h"
#include "qlog_optimize_stats.h"

#define QLOG_OPTIMIZE_TYPES \
  X(qlog_optimize_remove_identity_gates) \
  X(qlog_optimize_remove_paired_dup_gates) \
  X(qlog_optimize_remove_redundant_cnot)

typedef enum {
  REMOVE_IDENTITY_GATES
} qlog_opt_track_types;

typedef struct qlog_optimize_def {
  struct qlog_optimize_stats_def* qlog_optimize_stats;
  uint8_t* qlog_optimize_qubits;
  uint8_t qlog_optimize_size;
  struct qlog_def* qlog_optimize_qlog_ptr;
} qlog_optimize_def;

qlog_optimize_def* qlog_optimize_init(struct qlog_def* qlog);
void qlog_optimize_delete();
struct qlog_def* qlog_optimize_expand_multi_gate(struct qlog_def* qlog);
void qlog_optimize_expand_ccx(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry);
void qlog_optimize_expand_qft(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry);
void qlog_optimize_expand_rccx(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry);
void qlog_optimize_expand_rc3x(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry);
void qlog_optimize_expand_rxx(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry);
void qlog_optimize_expand_rzz(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry);
void qlog_optimize_expand_swap(struct qlog_def* qlog, struct qlog_entry_def* qlog_entry);
void qlog_optimize_remove_identity_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_remove_paired_dup_gates(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
void qlog_optimize_remove_redundant_cnot(struct qlog_def* qlog, struct qlog_def* optimized_qlog, qlog_optimize_def* qlog_optimize);
struct qlog_def* qlog_optimize_set(struct qlog_def* qlog);

