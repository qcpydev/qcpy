#include "qlog_optimize_graph.h"
#include "qlog_trigger_optimize.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

struct qlog_node_def* qlog_node_init(uint16_t id,
                                     struct qlog_entry_def* qlog_entry,
                                     struct qlog_node_def* qlog_node_prev,
                                     struct qlog_node_def* qlog_node_next,
                                     struct qlog_node_def* qlog_node_up,
                                     struct qlog_node_def* qlog_node_down) {

  struct qlog_node_def* qlog_node = (qlog_node_def*)malloc(sizeof(struct qlog_node_def));
  memset(qlog_node, 0, sizeof(qlog_node_def));
  qlog_node->qlog_node_id = id;
  qlog_node->qlog_node_prev = NULL;
  qlog_node->qlog_node_next = NULL;
  qlog_node->qlog_node_up = NULL;
  qlog_node->qlog_node_down = NULL;
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
    qlog_entry_delete(qlog_node->qlog_node_entry);
    qlog_node->qlog_node_entry = NULL;
  }

  free(qlog_node);
  qlog_node = NULL;
  return;
}

void qlog_node_dump_content(struct qlog_node_def* qlog_node, bool verbose) {
  if (!qlog_node || !qlog_node->qlog_node_entry) {
    return;
  }
  printf("qlog_node ptr: %p\n=======================\n", qlog_node);
  qlog_entry_dump_content(qlog_node->qlog_node_entry, true);
  printf("\n===================\n");
}

void qlog_graph_append(struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry) {
  if (!qlog_graph || !qlog_entry) {
    return;
  }

  if (qlog_entry->qlog_entry_qubit_cnt > qlog_graph->qlog_graph_size) {
    return;
  }

  struct qlog_entry_def* entry_copy = (struct qlog_entry_def*)malloc(sizeof(struct qlog_entry_def));
  memcpy(entry_copy, qlog_entry, sizeof(struct qlog_entry_def));

  uint8_t* qlog_entry_qubits = qlog_entry_deconstruct_qubit_flags(entry_copy);
  struct qlog_node_def** qlog_node_copies = (struct qlog_node_def**) malloc(sizeof(qlog_node_def*) * entry_copy->qlog_entry_qubit_cnt);

  for (uint8_t i = 0; i < qlog_entry->qlog_entry_qubit_cnt; ++i) {
    struct qlog_entry_def* qlog_entry_cpy = (struct qlog_entry_def*) malloc(sizeof(struct qlog_entry_def));
    memcpy(qlog_entry_cpy, entry_copy, sizeof(struct qlog_entry_def));

    uint8_t ins = qlog_entry_qubits[i];
    uint64_t id = qlog_entry_cpy->qlog_entry_id;

    struct qlog_node_def* insert = qlog_node_init(id, qlog_entry_cpy, qlog_graph->qlog_graph_circuit_track[ins], NULL, NULL, NULL);
    qlog_node_copies[i] = insert;

    if (!qlog_graph->qlog_graph_used_qubits[ins]) {
      qlog_graph->qlog_graph_circuit[ins] = insert;
      qlog_graph->qlog_graph_circuit_track[ins] = insert;
    }
    else {
      qlog_graph->qlog_graph_circuit_track[ins]->qlog_node_next = insert;
      insert->qlog_node_prev = qlog_graph->qlog_graph_circuit_track[ins];
      qlog_graph->qlog_graph_circuit_track[ins] = insert;
    }

    qlog_graph->qlog_graph_used_qubits[ins] = 1;

    if (i - 1 >= 0) {
      struct qlog_node_def* node_up = qlog_node_copies[i - 1];
      insert->qlog_node_up = node_up;
      node_up->qlog_node_down = insert;

      if (!qlog_graph->qlog_graph_used_qubits[qlog_entry_qubits[i - 1]]) {
        qlog_graph->qlog_graph_used_qubits[qlog_entry_qubits[i - 1]] = 1;
      }
    }
  }

  free(qlog_entry_qubits); 
  free(qlog_node_copies);
  qlog_entry_qubits = NULL;
 }


void qlog_graph_zero_and_set(struct qlog_graph_def* qlog_graph) {
  if (!qlog_graph) {
    return;
  }

  if (!qlog_graph->qlog_graph_size) {
    return;
  }

  for (uint16_t i = 0; i < qlog_graph->qlog_graph_size; ++i) {
    while (qlog_graph->qlog_graph_circuit_track[i] != qlog_graph->qlog_graph_circuit[i]) {
      struct qlog_node_def* temp_prev_node = qlog_graph->qlog_graph_circuit_track[i]->qlog_node_prev;
      qlog_node_delete(qlog_graph->qlog_graph_circuit_track[i]);
      qlog_graph->qlog_graph_circuit_track[i] = temp_prev_node;
    }

    qlog_graph->qlog_graph_row_size[i] = 0;
    qlog_graph->qlog_graph_used_qubits[i] = 0;
    qlog_graph->qlog_graph_circuit[i]->qlog_node_next = NULL;
  }
}

struct qlog_graph_def* qlog_graph_init(struct qlog_trigger_optimize_def* qlog_trig_opt) {
  if (!qlog_trig_opt) {
    return NULL;
  }

  qlog_graph_def* qlog_graph = (struct qlog_graph_def*)malloc(sizeof(struct qlog_graph_def));

  qlog_graph->qlog_graph_size = qlog_trig_opt->qlog_trigger_optimize_qubit_cnt;

  qlog_graph->qlog_graph_row_size = (uint16_t*) malloc(sizeof(uint16_t) * qlog_graph->qlog_graph_size); 
  bzero(qlog_graph->qlog_graph_row_size, sizeof(uint16_t) * qlog_graph->qlog_graph_size);

  qlog_graph->qlog_graph_used_qubits = (uint8_t*) malloc(sizeof(uint8_t) * qlog_graph->qlog_graph_size);
  bzero(qlog_graph->qlog_graph_used_qubits, sizeof(uint8_t) * qlog_graph->qlog_graph_size);

  qlog_graph->qlog_graph_circuit = (struct qlog_node_def**) malloc(sizeof(struct qlog_node_def*) * qlog_graph->qlog_graph_size);
  bzero(qlog_graph->qlog_graph_circuit, sizeof(struct qlog_node_def*) * qlog_graph->qlog_graph_size);

  qlog_graph->qlog_graph_circuit_track = (struct qlog_node_def**) malloc(sizeof(struct qlog_node_def*) * qlog_graph->qlog_graph_size);
  bzero(qlog_graph->qlog_graph_circuit_track, sizeof(struct qlog_node_def*) * qlog_graph->qlog_graph_size);

  return qlog_graph;
}

void qlog_graph_delete(struct qlog_graph_def* qlog_graph) {
  if (!qlog_graph) {
    return;
  }

  qlog_graph_zero_and_set(qlog_graph);

  free(qlog_graph->qlog_graph_circuit_track); 
  qlog_graph->qlog_graph_circuit_track = NULL;

  free(qlog_graph->qlog_graph_circuit);
  qlog_graph->qlog_graph_circuit = NULL;

  free(qlog_graph->qlog_graph_circuit);
  qlog_graph->qlog_graph_circuit = NULL;

  free(qlog_graph->qlog_graph_used_qubits);
  qlog_graph->qlog_graph_used_qubits = NULL;

  free(qlog_graph->qlog_graph_row_size);
  qlog_graph->qlog_graph_row_size = NULL;

  free(qlog_graph);
  qlog_graph = NULL;
}

void qlog_graph_dump_content(struct qlog_graph_def* qlog_graph, bool verbose) {
  if (!qlog_graph) {
    return;
  }

  printf("qlog_graph contents of: %p:\n"
         "Size: %d\n",
         qlog_graph,
         qlog_graph->qlog_graph_size);
  if (!qlog_graph->qlog_graph_used_qubits) {
    printf("used_qubits of qlog_graph not found!\n");
    return;
  }
  
  if (!qlog_graph->qlog_graph_circuit_track) {
    printf("circuit_track of qlog_graph not found!\n"); 
  }
  if (!qlog_graph->qlog_graph_size) {
    printf("qlog_graph is empty!\n");
    return;
  }

  for (uint8_t i = 0; i < qlog_graph->qlog_graph_size; ++i) {
    if (qlog_graph->qlog_graph_used_qubits[i]) {
      printf("qubit: %d in use\n", qlog_graph->qlog_graph_used_qubits[i]);
      qlog_node_def* node = qlog_graph->qlog_graph_circuit[i]->qlog_node_next;

      while (node) {
        qlog_node_dump_content(node, verbose); 
        node = node->qlog_node_next;
      } 
    }
  }
}
