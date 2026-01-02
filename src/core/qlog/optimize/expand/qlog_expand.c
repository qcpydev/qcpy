#include <assert.h>
#include <math.h>
#include <qlog_expand.h>
#include <qlog_entry.h>

typedef void (*qlog_expanders)(qlog_node_t*);

static const qlog_expanders qlog_expand_funcs[] = {
  qlog_expand_ccx,
  qlog_expand_qft,
  qlog_expand_rccx,
  qlog_expand_rc3x,
  qlog_expand_swap,
  qlog_expand_rxx,
  qlog_expand_rzz,
};

static const base_gate_e qlog_expand_gates[] = {
  GATE_CCX,
  GATE_QFT,
  GATE_RCCX,
  GATE_RC3X,
  GATE_SWAP,
  GATE_RXX,
  GATE_RZZ,
};

const int qlog_expand_gate_count = sizeof(qlog_expand_gates) /
             sizeof(qlog_expand_gates[0]);

static qlog_node_t** qlog_expand_get_node_arr(qlog_graph_t* qlog_graph) {
  assert(qlog_graph);

  qlog_node_t** qlog_node_arr = qlog_graph->nodes;

  if (qlog_graph->checkpointed) {
    qlog_node_arr = qlog_graph->node_checkpoint;

    for (uint16_t i = 0; i < qlog_graph->size; ++i) {
      if (qlog_node_arr[i] && qlog_node_arr[i]->next) {
        qlog_node_arr[i] = qlog_node_arr[i]->next;
      }
    }
  }

  assert(qlog_node_arr);
  return qlog_node_arr;
}

static bool qlog_expand_previous_gate_is_equal(qlog_node_t* qlog_node) {
  assert(qlog_node && qlog_node->qlog_entry);

  assert(qlog_node->qlog_entry->controlled_bitmask &&
         qlog_node->qlog_entry->target_bitmask);

  qlog_node_t* prev_node = qlog_node->prev;

  if (!prev_node) {
    return false;
  }

  assert(prev_node->qlog_entry);

  qlog_entry_t* prev_entry = prev_node->qlog_entry;
  qlog_entry_t* entry = qlog_node->qlog_entry;

  if (prev_entry->gate_type != entry->gate_type) {
    return false;
  }

  return qlog_entry_compare(prev_entry, entry);
}

static int qlog_expand_gate_valid(qlog_node_t* qlog_node) {
  assert(qlog_node);

  for (int i = 0; i < qlog_expand_gate_count; ++i) {
      if (qlog_expand_gates[i] == qlog_node->qlog_entry->gate_name) {
        return i;
      }
  }

  return -1;
}

void qlog_expand_qft(qlog_node_t* qlog_node) {
  assert(qlog_node);

}

void qlog_expand_rccx(qlog_node_t* qlog_node) {
  assert(qlog_node);
}

void qlog_expand_rc3x(qlog_node_t* qlog_node) {
  assert(qlog_node);
}

void qlog_expand_swap(qlog_node_t* qlog_node) {
  assert(qlog_node);
}

void qlog_expand_rxx(qlog_node_t* qlog_node) {
  assert(qlog_node);
}

void qlog_expand_rzz(qlog_node_t* qlog_node) {
  assert(qlog_node);
}

void qlog_expand_ccx(qlog_node_t* qlog_node) {
  assert(qlog_node);
  qlog_entry_t* qlog_entry = qlog_node->qlog_entry;

  uint16_t* controlled = qlog_entry_unpack(qlog_entry->controlled_count, qlog_entry->controlled_bitpack);
  uint16_t* target = qlog_entry_unpack(qlog_entry->target_count, qlog_entry->target_bitpack);

  free(controlled);
  free(target);
  controlled = NULL;
  target = NULL;
}

bool qlog_expand(qlog_graph_t* qlog_graph) {
  assert(qlog_graph);

  qlog_node_t** graph = qlog_expand_get_node_arr(qlog_graph);
  assert(graph);
  bool did_expand = false;

  uint16_t j = 0;
  while(j < qlog_graph->size && qlog_graph->node_count != 0) {
    qlog_node_t* starting_ptr = graph[j];
    while (graph[j]) {
      int is_valid = qlog_expand_gate_valid(graph[j]);
      if (is_valid != -1) {
        if (qlog_expand_previous_gate_is_equal(graph[j])) {
          qlog_graph_delete_node(graph, j);
          qlog_graph_delete_node(graph, j);
          qlog_graph->node_count -= 2;
          starting_ptr = graph[j];
        }
        else {
          did_expand = true;
          qlog_expand_funcs[is_valid](graph[j]);
          assert(graph[j]);
          qlog_graph_delete_node(graph, j);
          --qlog_graph->node_count;
          starting_ptr = graph[j];
        }
      }

      if (graph[j]) {
        graph[j] = graph[j]->next;
      }
    }

    j += 1;
    graph[j] = starting_ptr;
  }


  return did_expand;
}

