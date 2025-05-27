#include "qlog_trigger_optimize.h"
#include "qlog_optimize_shrink.h"

#define GATE_CAN_EXPAND(qg_name) (qg_name == GLOBAL_GATE_CCX  ||\
                                  qg_name == GLOBAL_GATE_QFT  ||\
                                  qg_name == GLOBAL_GATE_RCCX ||\
                                  qg_name == GLOBAL_GATE_RC3X ||\
                                  qg_name == GLOBAL_GATE_SWAP ||\
                                  qg_name == GLOBAL_GATE_RXX  ||\
                                  qg_name == GLOBAL_GATE_RZZ)

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
  qlog_trigger_optimize->qlog_trigger_optimize_parent->qlog_optimize->qlog_optimize_is_running = 1;
  
  
  return true;
}

struct qlog_trigger_optimize_def* qlog_trigger_optimize_init(struct qlog_def* qlog) {
  if (!qlog) {
    return NULL;
  }

  struct qlog_trigger_optimize_def* qlog_trigger_optimize = (struct qlog_trigger_optimize_def*) malloc(sizeof(struct qlog_trigger_optimize_def));
  qlog_trigger_optimize->qlog_trigger_optimize_parent = qlog;
  qlog_trigger_optimize->qlog_trigger_optimize_gate_cnt = qlog->qlog_size;
  qlog_trigger_optimize->qlog_trigger_optimize_subs = (struct qlog_trigger_optimize_sub_def**) malloc(sizeof(struct qlog_trigger_optimize_sub_def*) * QLOG_TRIGGER_SHRINK_MAX - 1);
  qlog_entry_def* entry_expand = (struct qlog_entry_def*) malloc(sizeof(struct qlog_entry_def));  

  qlog_trigger_optimize->qlog_trigger_optimize_qubit_cnt = qlog->qlog_qubit_cnt;
  qlog_trigger_optimize->qlog_trigger_optimize_expand_queue = qlog_entry_dummy_init();
  qlog_trigger_optimize->qlog_trigger_optimize_expand_queue_last = qlog_trigger_optimize->qlog_trigger_optimize_expand_queue;

  for (uint16_t i = 0; i < QLOG_TRIGGER_SHRINK_MAX; ++i) {
    qlog_trigger_optimize->qlog_trigger_optimize_subs[i] = qlog_trigger_optimize_init_arr[i](qlog_trigger_optimize);
    if (!qlog_trigger_optimize->qlog_trigger_optimize_subs[i]) {
      return NULL;
    }
  }

  qlog_trigger_optimize->qlog_trigger_optimize_graph = qlog_graph_init(qlog_trigger_optimize);  

  return qlog_trigger_optimize;
}

void qlog_trigger_optimize_delete(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {

}

void qlog_trigger_optimize_dump_log(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {

}

void qlog_trigger_optimize_append_entry(struct qlog_entry_def* qlog_entry, struct qlog_trigger_optimize_def* qlog_trig_opt) {
  if (!qlog_trig_opt) {
    return;
  }

  bool can_trigger = false;

  for (uint8_t i = 0; i < QLOG_TRIGGER_SHRINK_MAX; ++i) {
    struct qlog_trigger_optimize_sub_def* qlog_trig_sub = qlog_trig_opt->qlog_trigger_optimize_subs[i];

    if (qlog_trig_sub->qlog_trigger_optimize_sub_append(qlog_trig_sub,
                                                        qlog_trig_opt->qlog_trigger_optimize_graph,
                                                        qlog_entry)) {

      can_trigger = true;
    }
  }

  if (GATE_CAN_EXPAND(qlog_entry->qlog_entry_gate)) {
    struct qlog_entry_def* entry_expand = (struct qlog_entry_def*) malloc(sizeof(struct qlog_entry_def));
    memcpy(entry_expand, qlog_entry, sizeof(struct qlog_entry_def));
    qlog_trig_opt->qlog_trigger_optimize_expand_queue->qlog_entry_next = entry_expand;
    entry_expand->qlog_entry_prev = qlog_trig_opt->qlog_trigger_optimize_expand_queue;
    qlog_trig_opt->qlog_trigger_optimize_expand_queue = entry_expand;
  }
}

bool qlog_trigger_optimize_secure(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return false;
  }
  
  return true;
}

bool qlog_trigger_optimize_secure_graph(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return false;
  }

  return true;
}

bool qlog_trigger_optimize_can_do_min_work(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return false;
  }

  for (uint8_t i = 0; i < QLOG_TRIGGER_SHRINK_MAX; ++i) {
    if (qlog_trigger_optimize->qlog_trigger_optimize_subs[i]->qlog_trigger_optimize_sub_threshold_min > 0) {
      return true;
    }
  }

  return false;
}

