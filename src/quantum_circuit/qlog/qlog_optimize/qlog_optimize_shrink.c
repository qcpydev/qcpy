#include "qlog_optimize_shrink.h"

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_base(qlog_trigger_optimize_sub_trigger trigger_shrink_func,
                                                                          qlog_trigger_optimize_sub_shrink shrinker_func,
                                                                          qlog_trigger_optimize_shrink_types shrink_type, 
                                                                          uint16_t trigger_threshold_min,
                                                                          uint16_t trigger_threshold_max) {

  struct qlog_trigger_optimize_sub_def* qlog_trig_sub = (struct qlog_trigger_optimize_sub_def*)malloc(sizeof(struct qlog_trigger_optimize_sub_def));

  if (!qlog_trig_sub) {
    return NULL;
  }

  qlog_trig_sub->qlog_trigger_optimize_sub_entry_list = qlog_entry_dummy_init();
  qlog_trig_sub->qlog_trigger_optimize_sub_last = qlog_trig_sub->qlog_trigger_optimize_sub_entry_list;
  qlog_trig_sub->qlog_trigger_optimize_sub_append = trigger_shrink_func;
  qlog_trig_sub->qlog_trigger_optimize_sub_trigger = shrinker_func;
  qlog_trig_sub->qlog_trigger_optimize_sub_threshold_min = trigger_threshold_min;
  qlog_trig_sub->qlog_trigger_optimize_sub_threshold_max = trigger_threshold_max;
  qlog_trig_sub->qlog_trigger_optimize_sub_type = shrink_type;

  return qlog_trig_sub;
}


struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_unused_block(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_unused_block,
                                             &qlog_optimize_shrink_unused_block,
                                             QLOG_TRIGGER_SHRINK_UNUSED_BLOCK,
                                             0, 
                                             1);
}

qlog_optimize_shrink_sm qlog_optimize_shrink_unused_block(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return QLOG_OPTIMIZE_SHRINK_DONE;
}

bool qlog_optimize_shrink_trigger_unused_block(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return true;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_unused_multicontrolled(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_unused_multicontrolled,
                                             &qlog_optimize_shrink_unused_multicontrolled,
                                             QLOG_TRIGGER_SHRINK_UNUSED_MULTICONTROLLED,
                                             1, 
                                             2);
}

qlog_optimize_shrink_sm qlog_optimize_shrink_unused_multicontrolled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return QLOG_OPTIMIZE_SHRINK_DONE;
}

bool qlog_optimize_shrink_trigger_unused_multicontrolled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return false;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_hadamard_controlled(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_hadamard_controlled,
                                             &qlog_optimize_shrink_hadamard_controlled,
                                             QLOG_TRIGGER_SHRINK_HADAMARD_CONTROLLED,
                                             1, 
                                             4);
}

qlog_optimize_shrink_sm qlog_optimize_shrink_hadamard_controlled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return QLOG_OPTIMIZE_SHRINK_DONE;
}

bool qlog_optimize_shrink_trigger_hadamard_controlled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return false;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_unused_controlled(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_unused_controlled,
                                             &qlog_optimize_shrink_unused_controlled,
                                             QLOG_TRIGGER_SHRINK_UNUSED_CONTROLLED,
                                             1, 
                                             4);
}

qlog_optimize_shrink_sm qlog_optimize_shrink_unused_controlled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {

  return QLOG_OPTIMIZE_SHRINK_DONE;
}

bool qlog_optimize_shrink_trigger_unused_controlled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return true;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_duplicate_gates(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_duplicate_gates,
                                             &qlog_optimize_shrink_duplicate_gates,
                                             QLOG_TRIGGER_SHRINK_DUPLICATE_GATES,
                                             4, 
                                             10);
}

qlog_optimize_shrink_sm qlog_optimize_shrink_duplicate_gates(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return QLOG_OPTIMIZE_SHRINK_DONE;
}

bool qlog_optimize_shrink_trigger_duplicate_gates(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return false;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_identity_gates(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_identity_gates,
                                             &qlog_optimize_shrink_identity_gates,
                                             QLOG_TRIGGER_SHRINK_DUPLICATE_GATES,
                                             1, 
                                             50);
   
}

qlog_optimize_shrink_sm qlog_optimize_shrink_identity_gates(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return QLOG_OPTIMIZE_SHRINK_DONE;
}

bool qlog_optimize_shrink_trigger_identity_gates(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  return false;
}

void qlog_trigger_optimize_sub_delete(struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub);
void qlog_trigger_optimize_sub_dump_log(struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub);
