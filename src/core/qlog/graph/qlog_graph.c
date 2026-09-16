#include <assert.h>
#include <block.h>
#include <qlog.h>
#include <qlog_graph.h>
#include <qlog_optimize.h>
#include <stdlib.h>
#include <string.h>

qlog_graph_t *qlog_graph_init(uint16_t size) {
  qlog_graph_t *qlog_graph = NULL;
  qlog_graph = (qlog_graph_t *)malloc(sizeof(qlog_graph_t));
  assert(qlog_graph);
  memset(qlog_graph, 0, sizeof(qlog_graph_t));

  qlog_graph->size = size;

  qlog_graph->nodes = (qlog_node_t **)malloc(sizeof(qlog_node_t *) * size);

  assert(qlog_graph->nodes);

  qlog_graph->node_checkpoint =
      (qlog_node_t **)malloc(sizeof(qlog_node_t *) * size);

  qlog_graph->nodes_last = (qlog_node_t **)malloc(sizeof(qlog_node_t *) * size);

  for (uint16_t i = 0; i < size; ++i) {
    qlog_graph->nodes[i] = 0;
    qlog_graph->node_checkpoint[i] = 0;
    qlog_graph->nodes_last[i] = 0;
  }

  return qlog_graph;
}

void qlog_graph_update_checkpoint(qlog_graph_t *qlog_graph) {
  assert(qlog_graph);
  qlog_graph->checkpointed = true;
  for (uint64_t i = 0; i < qlog_graph->size; ++i) {
    qlog_graph->node_checkpoint[i] = qlog_graph->nodes_last[i];
  }
}

void qlog_graph_append(qlog_graph_t *qlog_graph, qlog_entry_t *qlog_entry) {
  assert(qlog_graph && qlog_entry);
  bitmask_t qubit_bitmask = qlog_entry->qubit_bitmask;
  bitmask_t get_qubit = 1;
  qubit_t qbt_count = qlog_entry->qubit_count;

  qubit_t i = 0;
  int last_seen = -1;

  while (i < QLOG_MAX_QUBITS && qbt_count != 0) {
    if (get_qubit & qubit_bitmask) {
      qlog_node_t *qlog_node =
          qlog_node_init(qlog_entry, qlog_graph->total_node_count, i, false);
      assert(qlog_node);

      if (!qlog_graph->nodes[i]) {
        qlog_graph->nodes[i] = qlog_node;
        qlog_graph->nodes_last[i] = qlog_node;
      } else {
        qlog_node->prev = qlog_graph->nodes_last[i];
        qlog_graph->nodes_last[i]->next = qlog_node;
        qlog_graph->nodes_last[i] = qlog_graph->nodes_last[i]->next;
      }

      if (((qlog_entry->controlled_bitmask > 0 &&
            qlog_entry->target_bitmask > 0) ||
           qlog_entry->qubit_count > 1) &&
          last_seen != -1) {

        qlog_graph->nodes_last[last_seen]->down = qlog_graph->nodes_last[i];
        qlog_graph->nodes_last[i]->up = qlog_graph->nodes_last[last_seen];
      }

      last_seen = i;
      --qbt_count;
    }

    get_qubit <<= 1;
    ++i;
  }

  ++qlog_graph->node_count;
  ++qlog_graph->total_node_count;

  if (qlog_graph->node_count % IMPORT_MAX_SIZE == 0 &&
      qlog_graph->node_count > 0) {
    if (qlog_optimize(qlog_graph, false)) {
      // coming back to this later for sake of headache...
      // qlog_graph_update_checkpoint(qlog_graph);
    }
  }
}

static void qlog_graph_delete_node_int(qlog_node_t **qlog_node_point,
                                       uint16_t idx, qlog_node_t *qlog_node) {
  assert(qlog_node_point && qlog_node);

  assert(qlog_node->prev != qlog_node && qlog_node->next != qlog_node);

  if (qlog_node->prev) {
    qlog_node->prev->next = qlog_node->next;

    if (qlog_node->next) {
      qlog_node->next->prev = qlog_node->prev;
    }

    qlog_node_point[idx] = qlog_node->prev;
  } else if (qlog_node->next) {
    qlog_node->next->prev = NULL;
    qlog_node_point[idx] = qlog_node->next;
  } else {
    memset(&qlog_node_point[idx], 0, sizeof(qlog_node_t));
    qlog_node_point[idx] = NULL;
  }

  assert(qlog_node_point[idx] != qlog_node);

  if (qlog_node->prev) {
    assert(qlog_node->prev->next != qlog_node);
    assert(qlog_node->prev == qlog_node_point[idx]);
  }
  if (qlog_node->next) {
    assert(qlog_node->next->prev != qlog_node);
  }
}

void qlog_graph_delete_node(qlog_node_t **qlog_node_point, uint16_t idx) {
  assert(qlog_node_point);

  qlog_node_t *qlog_node = qlog_node_point[idx];
  assert(qlog_node);

  qlog_node_t *qlog_node_up = NULL;
  qlog_node_t *qlog_node_down = NULL;
  uint16_t up_idx = 0;
  uint16_t down_idx = 0;

  if (qlog_node->up) {
    qlog_node_up = qlog_node->up;
    up_idx = qlog_node->up->qubit;
  }

  if (qlog_node->down) {
    qlog_node_down = qlog_node->down;
    down_idx = qlog_node->down->qubit;
  }

  assert(up_idx < down_idx);

  while (qlog_node_up || qlog_node_down) {
    if (qlog_node_down) {
      down_idx = qlog_node_down->qubit;
      qlog_graph_delete_node_int(qlog_node_point, down_idx, qlog_node_down);
      qlog_node_down = qlog_node_down->down;

      if (qlog_node_point[down_idx] != 0) {
        assert(qlog_node_point[down_idx] != qlog_node_down);
      }
    }

    if (qlog_node_up) {
      up_idx = qlog_node_up->qubit;
      qlog_graph_delete_node_int(qlog_node_point, up_idx, qlog_node_up);
      qlog_node_up = qlog_node_up->up;

      if (qlog_node_point[up_idx] != 0) {
        assert(qlog_node_point[up_idx] != qlog_node_up);
      }
    }
  }

  qlog_graph_delete_node_int(qlog_node_point, idx, qlog_node);
  qlog_node_delete(qlog_node);
}

void qlog_graph_get_batch(qlog_node_t **qlog_nodes, uint32_t size) {
  assert(qlog_nodes);

  for (uint32_t i = 0; i < size; ++i) {
    if (qlog_nodes[i]) {
      qlog_nodes[i] = qlog_nodes[i]->next;
    }
  }
}

static inline qlog_node_t *
qlog_graph_insert_get_iter_qubit(qlog_node_t *qlog_node,
                                 qubit_t to_insert_qubit) {
  assert(qlog_node);

  if (qlog_node->qubit == to_insert_qubit) {
    return qlog_node;
  }

  qlog_node_t *node_up = qlog_node->up;
  qlog_node_t *node_down = qlog_node->down;

  qlog_node_t *found_node = NULL;

  while (node_up || node_down) {
    if (node_up && node_up->qubit == to_insert_qubit) {
      return node_up;
    } else if (node_down && node_down->qubit == to_insert_qubit) {
      return node_down;
    }

    if (node_up) {
      node_up = node_up->up;
    }

    if (node_down) {
      node_down = node_down->down;
    }
  }

  assert(!"No node was found, which is strange and something has gone horribly "
          "wrong.");
  return NULL;
}

static inline void qlog_graph_stitch_node(qlog_node_t *begin_node,
                                          qlog_node_t *to_switch,
                                          qlog_node_t *end_node) {
  assert(begin_node && to_switch);

  begin_node->next = to_switch;
  to_switch->prev = begin_node;

  if (end_node) {
    to_switch->next = end_node;
    end_node->prev = to_switch;
  }
}

void qlog_graph_insert(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node,
                       qlog_entry_t *qlog_entry) {
  assert(qlog_graph && qlog_node);
  bitmask_t i = 1;

  for (qubit_t qubit = 0; qubit < QLOG_MAX_QUBITS; ++qubit) {
    if (i & qlog_entry->qubit_bitmask) {
      qlog_node_t *curr_node =
          qlog_graph_insert_get_iter_qubit(qlog_node, qubit);

      qlog_node_t *to_insert =
          qlog_node_init(qlog_entry, qlog_graph->total_node_count, qubit, true);

      qlog_graph_stitch_node(curr_node, to_insert, curr_node->next);
    }

    i <<= 1;
  }

  ++qlog_graph->total_node_count;
  ++qlog_graph->node_count;
}
