#include "qlog_optimize_shrink.h"
#include "qlog_optimize_expand.h"
#include "qlog_trigger_optimize.h"
#include <string.h>

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_base(qlog_trigger_optimize_sub_trigger trigger_shrink_func,
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
                                             QLOG_TRIGGER_SHRINK_UNUSED_BLOCK,
                                             0, 
                                             1);
}

bool qlog_optimize_shrink_trigger_unused_block(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry) {
  // this is the same process as unused_controlled
  // the only difference is that if the gate is in fact being used,
  // then it must be expanded and processed
  return false;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_unused_multicontrolled(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_unused_multicontrolled,
                                             QLOG_TRIGGER_SHRINK_UNUSED_MULTICONTROLLED,
                                             1, 
                                             2);
}


bool qlog_optimize_shrink_trigger_unused_multicontrolled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry) {
  // this is the same process as unused_controlled
  // the only difference is that if the gate is in fact being used,
  // then it must be expanded and processed
  //
  return false;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_hadamard_controlled(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_hadamard_controlled,
                                             QLOG_TRIGGER_SHRINK_HADAMARD_CONTROLLED,
                                             1, 
                                             4);
}

bool qlog_optimize_shrink_trigger_hadamard_controlled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry) {
  if (qlog_trig_sub || qlog_graph || qlog_entry) {
    return false;
  }

  if (qlog_entry->qlog_entry_gate_type != GLOBAL_GATE_HADAMARD ||
      qlog_graph->qlog_graph_size - 2 > 0) {
    return false;
  } 

  qlog_node_def* entry_check = qlog_graph->qlog_graph_circuit_track[qlog_entry->qlog_entry_qubits]; 
  qlog_node_def* controlled_node;

  if(entry_check->qlog_node_entry->qlog_entry_gate == GLOBAL_GATE_CX &&
     entry_check->qlog_node_prev->qlog_node_entry->qlog_entry_gate == GLOBAL_GATE_HADAMARD) {

    if (entry_check->qlog_node_down) {
      controlled_node = entry_check->qlog_node_down;
    }

    else if (entry_check->qlog_node_up){
      controlled_node = entry_check->qlog_node_up;
    }

    else {
      // panic
    }

    if (controlled_node->qlog_node_prev->qlog_node_entry->qlog_entry_gate == GLOBAL_GATE_HADAMARD &&
        controlled_node->qlog_node_next->qlog_node_entry->qlog_entry_gate == GLOBAL_GATE_HADAMARD) { 
      ++qlog_trig_sub->qlog_trigger_optimize_sub_pattern_cnt;
      qlog_trig_sub->qlog_trigger_optimize_sub_gate_cnt += 5; 
      struct qlog_entry_def* controlled_node_copy_1;
      struct qlog_entry_def* controlled_node_copy_2;
      struct qlog_entry_def* controlled_node_copy_3;
      struct qlog_entry_def* controlled_node_copy_4;
      struct qlog_entry_def* controlled_node_copy_5;

      memcpy(controlled_node_copy_1, controlled_node->qlog_node_prev->qlog_node_entry, sizeof(struct qlog_entry_def));
      memcpy(controlled_node_copy_2, controlled_node->qlog_node_entry, sizeof(struct qlog_entry_def));
      memcpy(controlled_node_copy_3, controlled_node->qlog_node_next->qlog_node_entry, sizeof(struct qlog_entry_def));
      memcpy(controlled_node_copy_4, qlog_entry, sizeof(struct qlog_entry_def));
      memcpy(controlled_node_copy_5, entry_check->qlog_node_prev, sizeof(struct qlog_entry_def));

      qlog_trig_sub->qlog_trigger_optimize_sub_last->qlog_entry_next = controlled_node_copy_5;
      controlled_node_copy_5->qlog_entry_prev = qlog_trig_sub->qlog_trigger_optimize_sub_last;
      controlled_node_copy_5->qlog_entry_next = controlled_node_copy_4; 
      controlled_node_copy_4->qlog_entry_prev = controlled_node_copy_5;
      controlled_node_copy_3->qlog_entry_prev = controlled_node_copy_4;
      controlled_node_copy_3->qlog_entry_next = controlled_node_copy_2;
      controlled_node_copy_2->qlog_entry_prev = controlled_node_copy_3;
      controlled_node_copy_2->qlog_entry_next = controlled_node_copy_1;
      controlled_node_copy_1->qlog_entry_prev = controlled_node_copy_1;

      qlog_trig_sub->qlog_trigger_optimize_sub_last = controlled_node_copy_1;
    }

    if (qlog_trig_sub->qlog_trigger_optimize_sub_pattern_cnt >= qlog_trig_sub->qlog_trigger_optimize_sub_threshold_max) {
      return true;
    }
  }

  return false;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_unused_controlled(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_unused_controlled,
                                             QLOG_TRIGGER_SHRINK_UNUSED_CONTROLLED,
                                             1, 
                                             4);
}

bool qlog_optimize_shrink_trigger_unused_controlled(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry) {
  if (!qlog_trig_sub || !qlog_graph || !qlog_entry) {
    return false;
  }

  // need to use matrix multiplication eventually.
  // basically start from the beginning of the qlog_control and qlog_target lists, and determine 
  // if we can iterate till such gate, where we see if the gate has been activated or not.

  uint8_t* qubits = qlog_entry_deconstruct_qubit_flags(qlog_entry);
  if (qlog_graph->qlog_graph_row_size[qubits[0]] == 0 && qlog_graph->qlog_graph_row_size[qubits[1]]) {
    struct qlog_entry_def* qlog_control = qlog_graph->qlog_graph_circuit_track[qubits[0]]->qlog_node_entry;
    struct qlog_entry_def* qlog_target = qlog_graph->qlog_graph_circuit_track[qubits[1]]->qlog_node_entry;
  }

  if (qlog_trig_sub->qlog_trigger_optimize_sub_pattern_cnt >= qlog_trig_sub->qlog_trigger_optimize_sub_threshold_max) {
    return true;
  }

  return false;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_duplicate_gates(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_duplicate_gates,
                                             QLOG_TRIGGER_SHRINK_DUPLICATE_GATES,
                                             1, 
                                             5);
}

bool qlog_optimize_shrink_trigger_duplicate_gates(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry) {
  if (!qlog_trig_sub || !qlog_graph || !qlog_entry) {
    return false;
  }

  uint8_t* qubits = qlog_entry_deconstruct_qubit_flags(qlog_entry);

  for (uint16_t i = 0; i < qlog_entry->qlog_entry_qubit_cnt; ++i) {
    if (!qlog_entry_compare_entries(qlog_entry, qlog_graph->qlog_graph_circuit_track[i]->qlog_node_entry)) {
      return false;
    }

    qlog_entry_def* prev_entry = qlog_graph->qlog_graph_circuit_track[i]->qlog_node_entry;

    if (prev_entry->qlog_entry_prev && !qlog_entry_compare_entries(prev_entry, prev_entry->qlog_entry_prev)) {
      qlog_entry_def* copy_qlog_entry;
      qlog_entry_def* copy_prev_entry;

      memcpy(copy_prev_entry, prev_entry, sizeof(qlog_entry_def));
      memcpy(copy_qlog_entry, qlog_entry, sizeof(qlog_entry_def));

      copy_prev_entry->qlog_entry_next = copy_qlog_entry;
      copy_prev_entry->qlog_entry_prev = qlog_trig_sub->qlog_trigger_optimize_sub_last;
      copy_qlog_entry->qlog_entry_prev = copy_prev_entry;
      qlog_trig_sub->qlog_trigger_optimize_sub_last = copy_qlog_entry;

      ++qlog_trig_sub->qlog_trigger_optimize_sub_pattern_cnt;
      qlog_trig_sub->qlog_trigger_optimize_sub_gate_cnt += 2;
    }
  }

  if (qlog_trig_sub->qlog_trigger_optimize_sub_pattern_cnt >= qlog_trig_sub->qlog_trigger_optimize_sub_threshold_max) {
    for(qlog_entry_def* qlog_entry_temp = qlog_entry;
        qlog_entry_clean_duplicate_chains(qlog_entry_temp);
        qlog_entry_temp = qlog_entry_temp->qlog_entry_prev);
    return true;
  } 

  return false;
}

struct qlog_trigger_optimize_sub_def* qlog_trigger_optimize_sub_init_identity_gates(struct qlog_trigger_optimize_def* qlog_trigger_optimize) {
  if (!qlog_trigger_optimize) {
    return NULL;
  }
  
  return qlog_trigger_optimize_sub_init_base(&qlog_optimize_shrink_trigger_identity_gates,
                                             QLOG_TRIGGER_SHRINK_DUPLICATE_GATES,
                                             1, 
                                             50);
}

bool qlog_optimize_shrink_trigger_identity_gates(struct qlog_trigger_optimize_sub_def* qlog_trig_sub, struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry) {
  if (!qlog_trig_sub || !qlog_entry) {
    return false;
  }
  
  if (!qlog_trig_sub->qlog_trigger_optimize_sub_entry_list) {
    return false;
  }

  if (!(qlog_entry->qlog_entry_gate_type == GLOBAL_GATE_IDENTITY)) {
    return false;
  }

  struct qlog_entry_def* copy_entry = memcpy(copy_entry, qlog_entry, sizeof(struct qlog_entry_def));

  copy_entry->qlog_entry_prev = qlog_trig_sub->qlog_trigger_optimize_sub_last->qlog_entry_prev;
  qlog_trig_sub->qlog_trigger_optimize_sub_last->qlog_entry_next = copy_entry;

  qlog_trig_sub->qlog_trigger_optimize_sub_last = copy_entry;
  ++qlog_trig_sub->qlog_trigger_optimize_sub_pattern_cnt; 
  ++qlog_trig_sub->qlog_trigger_optimize_sub_gate_cnt;

  return qlog_trig_sub->qlog_trigger_optimize_sub_pattern_cnt >= qlog_trig_sub->qlog_trigger_optimize_sub_threshold_max;
}

void qlog_trigger_optimize_sub_delete(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  if (!qlog_trig_sub) {
    return;
  }
  qlog_entry_def* qlog_next = qlog_trig_sub->qlog_trigger_optimize_sub_entry_list;
  qlog_entry_def* qlog_next_temp = qlog_next->qlog_entry_next;

  while (qlog_next) {
    qlog_entry_delete(qlog_next);
    qlog_next = qlog_next_temp;
    qlog_next_temp = qlog_next_temp->qlog_entry_next;
  }

  free(qlog_trig_sub);
  qlog_trig_sub = NULL;
}

void qlog_trigger_optimize_sub_dump_log(struct qlog_trigger_optimize_sub_def* qlog_trig_sub) {
  if (qlog_trig_sub) {
    return;
  }

}
