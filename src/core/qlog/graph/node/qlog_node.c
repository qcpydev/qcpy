#include <assert.h>
#include <string.h>
#include <qlog_node.h>


qlog_node_t* qlog_node_init(qlog_entry_t* qlog_entry, uint64_t id, uint16_t qubit) {
  assert(qlog_entry);
  qlog_node_t* qlog_node = (qlog_node_t*)malloc(sizeof(qlog_node_t));
  assert(qlog_node);
  memset(qlog_node, 0, sizeof(qlog_node_t));
  qlog_node->qlog_entry = qlog_entry;
  qlog_node->id = id;
  qlog_node->qubit = qubit;
  return qlog_node;
}

void qlog_node_delete(qlog_node_t* qlog_node) {
  assert(qlog_node);
  qlog_node_t* qlog_node_up = qlog_node->up;
  qlog_node_t* qlog_node_down = qlog_node->down;
  qlog_node_t* temp_delete = NULL;

  while (qlog_node_up || qlog_node_down) {
    if (qlog_node_up) {
      temp_delete = qlog_node_up;
      qlog_node_up = qlog_node_up->up;
      free(temp_delete);
      temp_delete = NULL;
    }

    if (qlog_node_down) {
      temp_delete = qlog_node_down;
      qlog_node_down = qlog_node_down->down;
      free(temp_delete);
      temp_delete = NULL;
    }
  }

  free(qlog_node);
  qlog_node = NULL;
}
