#include <qlog_entry.h>

#pragma once
#ifndef QLOG_NODE_H
#define QLOG_NODE_H

/*
 * qlog nodes make up a qlog graph
 */

typedef struct qlog_node_t qlog_node_t;
struct qlog_node_t {
  qlog_node_t* next;
  qlog_node_t* prev;
  qlog_node_t* up;
  qlog_node_t* down;
  qlog_entry_t* qlog_entry;
  uint64_t id;
  uint16_t qubit;
};

qlog_node_t* qlog_node_init(qlog_entry_t* qlog_entry, uint64_t id, uint16_t qubit);
void qlog_node_delete(qlog_node_t* qlog_node);

#endif
