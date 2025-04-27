#include "qlog_trigger_optimize.h"

typedef struct qlog_trigger_optimize_sub_def* (*qlog_trigger_optimize_sub_inits)(struct qlog_trigger_optimize_def*);
qlog_trigger_optimize_sub_inits qlog_trigger_optimize_init_arr[] = {
  [QLOG_TRIGGER_SHRINK_UNUSED_BLOCK] = qlog_trigger_optimize_sub_init_unused_block,
  [QLOG_TRIGGER_SHRINK_UNUSED_MULTICONTROLLED] = qlog_trigger_optimize_sub_init_unused_multicontrolled,
  [QLOG_TRIGGER_SHRINK_HADAMARD_CONTROLLED] = qlog_trigger_optimize_sub_init_hadamard_controlled,
  [QLOG_TRIGGER_SHRINK_UNUSED_CONTROLLED] = qlog_trigger_optimize_sub_init_unused_controlled,
  [QLOG_TRIGGER_SHRINK_DUPLICATE_GATES] = qlog_trigger_optimize_sub_init_duplicate_gates,
  [QLOG_TRIGGER_SHRINK_IDENTITY_GATES] = qlog_trigger_optimize_sub_init_identity_gates, 
};

bool qlog_trigger_optimize(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return false;
  }
  
  return true;
}

struct qlog_trigger_optimize_def* qlog_trigger_optimize_init(struct qlog_def* qlog) {
  if (!qlog) {
    return NULL;
  }

  struct qlog_trigger_optimize_def* qlog_trigger_optimize = (struct qlog_trigger_optimize_def*) malloc(sizeof(struct qlog_trigger_optimize_def));
  qlog_trigger_optimize->qlog_trigger_optimize_parent = qlog;
  //qlog_trigger_optimize->qlog_trigger_optimize_graph = qlog_trigger_optimize_graph_init()  
  qlog_trigger_optimize->qlog_trigger_optimize_gate_cnt = qlog->qlog_size;
  qlog_trigger_optimize->qlog_trigger_optimize_subs = (struct qlog_trigger_optimize_sub_def**) malloc(sizeof(struct qlog_trigger_optimize_sub_def*) * QLOG_TRIGGER_SHRINK_MAX - 1);
  
  for (uint16_t i = 0; i < QLOG_TRIGGER_SHRINK_MAX - 1; ++i) {
    qlog_trigger_optimize->qlog_trigger_optimize_subs[i] = qlog_trigger_optimize_init_arr[i](qlog_trigger_optimize);
    if (!qlog_trigger_optimize->qlog_trigger_optimize_subs[i]) {
      return NULL;
    }
  }

  return qlog_trigger_optimize;
}
void qlog_trigger_optimize_delete(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
void qlog_trigger_optimize_dump_log(struct qlog_trigger_optimize_def* qlog_trigger_optimize);

void qlog_trigger_optimize_append_entry(struct qlog_entry_def* qlog_entry, struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  return;
}

bool qlog_trigger_optimize_secure(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_trigger_optimize_secure_graph(struct qlog_trigger_optimize_def* qlog_trigger_optimize);
bool qlog_trigger_optimize_can_do_work(struct qlog_trigger_optimize_def* qlog_trigger_optimize);

