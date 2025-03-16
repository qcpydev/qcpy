#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "qlog_optimize.h"

typedef struct qlog_node_def {
  struct qlog_entry_def* qlog_node_entry;
  struct qlog_node_def* qlog_node_prev;
  struct qlog_node_def* qlog_node_next;
  struct qlog_node_def* qlog_node_up;
  struct qlog_node_def* qlog_node_down;
} qlog_node_def;

typedef struct qlog_graph_def {
  uint8_t qlog_graph_size;
  uint8_t* qlog_graph_used_qubits;
  uint16_t* qlog_graph_row_size;
  struct qlog_node_def** qlog_graph_circuit;
  struct qlog_node_def** qlog_graph_circuit_track;
} qlog_graph_def;

struct qlog_node_def* qlog_node_init(struct qlog_entry_def* qlog_entry, struct qlog_node_def* qlog_node_prev, struct qlog_node_def* qlog_node_next, struct qlog_node_def* qlog_node_up, struct qlog_node_def* qlog_node_down);
void qlog_node_delete(struct qlog_node_def* qlog_node);
void qlog_node_dump_content(struct qlog_node_def* qlog_node);


void qlog_graph_append(struct qlog_graph_def* qlog_graph, struct qlog_entry_def* qlog_entry, uint8_t ins, bool controlled, uint8_t ins_targ);
void qlog_graph_zero_and_set(struct qlog_graph_def* qlog_graph);
struct qlog_graph_def* qlog_graph_init(qlog_optimize_def* qlog_optimize);
void qlog_graph_delete(struct qlog_graph_def* qlog_graph);
void qlog_graph_dump_content(struct qlog_graph_def* qlog_graph);
