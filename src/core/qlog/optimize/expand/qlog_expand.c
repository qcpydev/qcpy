#include <assert.h>
#include <base.h>
#include <qlog_entry.h>
#include <qlog_expand.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*qlog_expanders)(qlog_graph_t *, qlog_node_t *);

static const qlog_expanders qlog_expand_funcs[GATE_MAX] = {
    [GATE_CCX] = qlog_expand_ccx,   [GATE_QFT] = qlog_expand_qft,
    [GATE_RCCX] = qlog_expand_rccx, [GATE_RC3X] = qlog_expand_rc3x,
    [GATE_SWAP] = qlog_expand_swap, [GATE_RXX] = qlog_expand_rxx,
    [GATE_RZZ] = qlog_expand_rzz,
};

static const bool qlog_expand_gates[GATE_MAX] = {
    [GATE_CCX] = true,  [GATE_QFT] = true,  [GATE_RCCX] = true,
    [GATE_RC3X] = true, [GATE_SWAP] = true, [GATE_RXX] = true,
    [GATE_RZZ] = true,
};

const int qlog_expand_gate_count =
    sizeof(qlog_expand_gates) / sizeof(qlog_expand_gates[0]);

static qlog_node_t **qlog_expand_get_node_arr(qlog_graph_t *qlog_graph) {
  assert(qlog_graph);

  qlog_node_t **qlog_node_arr = qlog_graph->nodes;

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

static bool qlog_expand_previous_gate_is_equal(qlog_node_t *qlog_node) {
  assert(qlog_node && qlog_node->qlog_entry);

  assert(qlog_node->qlog_entry->controlled_bitmask &&
         qlog_node->qlog_entry->target_bitmask);

  qlog_node_t *prev_node = qlog_node->prev;

  if (!prev_node) {
    return false;
  }

  assert(prev_node->qlog_entry);

  qlog_entry_t *prev_entry = prev_node->qlog_entry;
  qlog_entry_t *entry = qlog_node->qlog_entry;

  if (prev_entry->gate_type != entry->gate_type) {
    return false;
  }

  return qlog_entry_compare(prev_entry, entry);
}

void qlog_expand_qft(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node) {}
void qlog_expand_rccx(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node) {}
void qlog_expand_rc3x(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node) {}
void qlog_expand_swap(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node) {}
void qlog_expand_rzz(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node) {}
void qlog_expand_rxx(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node) {}

void qlog_expand_ccx(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node) {
  assert(qlog_node);
  qlog_entry_t *qlog_entry = qlog_node->qlog_entry;

  assert(qlog_entry->qubit_count == 3 && qlog_entry->controlled_count == 2 &&
         qlog_entry->target_count == 1);

  qubit_t *controlled = base_decompress_qubit_bitpack(
      qlog_entry->controlled_count, qlog_entry->controlled_bitpack);

  qubit_t *target = base_decompress_qubit_bitpack(qlog_entry->target_count,
                                                  qlog_entry->target_bitpack);

  qubit_t control_one = controlled[0];
  qubit_t control_two = controlled[1];
  qubit_t target_qubit = target[0];

  qlog_entry_init_params_t qlog_params_hadamard_one = {.qubits = target,
                                                       .qubit_count = 1};

  qlog_entry_t *qlog_entry_h =
      qlog_entry_init_hadamard_gate(&qlog_params_hadamard_one);

  qubit_t qubits_cx_one[2] = {control_two, target[0]};
  qubit_t qubits_cx_one_controlled[1] = {control_two};
  qlog_entry_init_params_t qlog_params_cx_one = {.qubits = qubits_cx_one,
                                                 .qubit_count = 2,
                                                 .controls =
                                                     qubits_cx_one_controlled,
                                                 .controlled_count = 1,
                                                 .targets = target,
                                                 .target_count = 1};

  qlog_entry_t *qlog_entry_cx = qlog_entry_init_cx_gate(&qlog_params_cx_one);

  qlog_graph_insert(qlog_graph, qlog_node, qlog_entry_cx);
  qlog_graph_insert(qlog_graph, qlog_node, qlog_entry_h);
  printf("YUM:%lu, NODES REMOVED: %lu\n", qlog_graph->node_count,
         qlog_graph->total_nodes_removed);

  free(controlled);
  free(target);
}

bool qlog_expand(qlog_graph_t *qlog_graph) {
  assert(qlog_graph);

  qlog_node_t **graph = qlog_expand_get_node_arr(qlog_graph);
  assert(graph);
  bool did_expand = false;
  qubit_t j = 0;

  while (j < qlog_graph->size && qlog_graph->node_count != 0) {
    qlog_node_t *starting_ptr = graph[j];

    while (graph[j]) {
      if (qlog_expand_gates[graph[j]->qlog_entry->gate_name]) {
        if (qlog_expand_previous_gate_is_equal(graph[j])) {
          qlog_graph_delete_node(graph, j);
          qlog_graph_delete_node(graph, j);

          qlog_graph->total_nodes_removed += 2;
          qlog_graph->node_count -= 2;
          starting_ptr = graph[j];
        } else {
          did_expand = true;
          qlog_expand_funcs[graph[j]->qlog_entry->gate_name](qlog_graph,
                                                             graph[j]);
          assert(&graph[j]);
          qlog_graph_delete_node(graph, j);

          ++qlog_graph->total_nodes_removed;
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
