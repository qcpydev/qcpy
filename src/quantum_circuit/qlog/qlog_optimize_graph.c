#include "qlog_optimize_graph.h"
#include <stdio.h>

struct qlog_node_def* qlog_node_init(struct qlog_entry_def* qlog_entry,
                                     struct qlog_node_def* qlog_node_prev,
                                     struct qlog_node_def* qlog_node_next,
                                     struct qlog_node_def* qlog_node_up,
                                     struct qlog_node_def* qlog_node_down) {
  struct qlog_node_def* qlog_node = (qlog_node_def*)malloc(sizeof(struct qlog_node_def)); 
  if (qlog_entry) {
    qlog_node->qlog_node_entry = qlog_entry;
  }
  
  if (qlog_node_prev) {
    qlog_node->qlog_node_prev = qlog_node_prev;
  }

  if (qlog_node_next) {
    qlog_node->qlog_node_next = qlog_node_next;
  }

  if (qlog_node_up) {
    qlog_node->qlog_node_up = qlog_node_up;
  }

  if (qlog_node_down) {
    qlog_node->qlog_node_down = qlog_node_down;
  }

  return qlog_node;
}

void qlog_node_delete(struct qlog_node_def* qlog_node) {
  if (!qlog_node) {
    return; 
  }
  if (qlog_node->qlog_node_entry) {
    free(qlog_node->qlog_node_entry);
    qlog_node->qlog_node_entry = NULL;
  }

  if (qlog_node->qlog_node_next) {
    qlog_node->qlog_node_next = NULL;
  }

  if (qlog_node->qlog_node_prev) {
    qlog_node->qlog_node_prev = NULL;
  }

  if (qlog_node->qlog_node_up) {
    qlog_node->qlog_node_up = NULL;
  }

  if (qlog_node->qlog_node_down) {
    qlog_node->qlog_node_down = NULL;
  }

  free(qlog_node);
  qlog_node = NULL;
  return;
}

void qlog_node_dump_content(struct qlog_node_def* qlog_node) {
  if (!qlog_node) {
    return;
  }
}

void qlog_graph_append(struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry, uint8_t ins, bool controlled, uint8_t ins_targ) {
  if (!qlog_graph) {
    return;
  }

  if (ins < 0 || ins > qlog_graph->qlog_graph_size) {
    return;
  }

  struct qlog_node_def* insert = qlog_node_init(qlog_entry, qlog_graph->qlog_graph_circuit_track[ins], NULL, NULL, NULL);
  qlog_graph->qlog_graph_circuit_track[ins]->qlog_node_next = insert;

  if (controlled) {
    struct qlog_node_def* paired_insert = qlog_node_init(qlog_entry, qlog_graph->qlog_graph_circuit_track[ins_targ], NULL, insert, NULL);
    insert->qlog_node_down = paired_insert;
    qlog_graph->qlog_graph_circuit_track[ins_targ]->qlog_node_next = paired_insert;
    qlog_graph->qlog_graph_circuit_track[ins_targ] = paired_insert;
  } 

  qlog_graph->qlog_graph_circuit_track[ins] = insert;
}


void qlog_graph_zero_and_set(struct qlog_graph_def* qlog_graph) {
  if (!qlog_graph) {
    return;
  }


}

struct qlog_graph_def* qlog_graph_init(qlog_optimize_def* qlog_optimize) {
  if (!qlog_optimize) {
    return NULL;
  }

  qlog_graph_def* qlog_graph = (struct qlog_graph_def*)malloc(sizeof(struct qlog_graph_def));

  qlog_graph->qlog_graph_size = qlog_optimize->qlog_optimize_size;
  qlog_graph->qlog_graph_row_size = (uint16_t*) malloc(sizeof(uint16_t) * qlog_optimize->qlog_optimize_size); 
  qlog_graph->qlog_graph_used_qubits = (uint8_t*) malloc(sizeof(uint8_t) * qlog_optimize->qlog_optimize_size);
  qlog_graph->qlog_graph_circuit = (struct qlog_node_def**) malloc(sizeof(struct qlog_node_def*) * qlog_optimize->qlog_optimize_size);
  qlog_graph->qlog_graph_circuit_track = (struct qlog_node_def**) malloc(sizeof(struct qlog_node_def*) * qlog_optimize->qlog_optimize_size);

  for (uint8_t i = 0; i < qlog_graph->qlog_graph_size; ++i) {
    qlog_graph->qlog_graph_circuit[i] = qlog_node_init(NULL, NULL, NULL, NULL, NULL);
    qlog_graph->qlog_graph_circuit_track[i] = qlog_graph->qlog_graph_circuit[i];
  }

  for (uint16_t i = 0; i < qlog_optimize->qlog_optimize_size; ++i) {
    struct qlog_entry_def* qlog_new_entry;
    memcpy(qlog_new_entry, qlog_optimize->qlog_optimize_qlog_ptr->qlog_entries[i], sizeof(struct qlog_entry_def));
    ++qlog_graph->qlog_graph_row_size[i];
    qlog_graph->qlog_graph_used_qubits[i] = 1;

    if (qlog_new_entry->qlog_entry_qubit_cnt > 1) {
      qlog_graph_append(qlog_graph, qlog_new_entry, qlog_new_entry->qlog_entry_qubits[0], true, qlog_new_entry->qlog_entry_qubits[1]);
    }
    else {
      qlog_graph_append(qlog_graph, qlog_new_entry, qlog_new_entry->qlog_entry_qubits[0], false, 0);
    }

  }

  return qlog_graph;
}

void qlog_graph_delete(struct qlog_graph_def* qlog_graph) {
  if (!qlog_graph) {
    return;
  }
}
