#include <qlog_node.h>
#include <qlog_entry.h>
#include <stdint.h>
#include <string.h>

#pragma once
#ifndef QLOG_GRAPH_H
#define QLOG_GRAPH_H

/*
 * qlog graph is a 2D quantum circuit structure of a qlog that is made up of
 * qlog nodes. This structure will be sent over to quack when deemed
 * ready/compressed/etc. The optimization engine will interact with this
 * structure. Each time that we optimize, we want to update the checkpoint, which
 * will be ignored whenever the user requests the state of the quantum circuit.
 */

typedef struct qlog_graph_s {
  uint64_t node_count;
  uint64_t total_node_count;
  qlog_node_t** nodes;
  qlog_node_t** nodes_last;
  qlog_node_t** node_checkpoint;
  uint16_t size;
  bool checkpointed;
} qlog_graph_t;

qlog_graph_t* qlog_graph_init(uint16_t size);
void qlog_graph_append(qlog_graph_t* qlog_graph, qlog_entry_t* qlog_entry);
void qlog_graph_delete_node(qlog_node_t** qlog_node, uint16_t idx);
void qlog_graph_update_checkpoint(qlog_graph_t* qlog_graph);
#endif
