#include <qlog_graph.h>

#pragma once
#ifndef QLOG_EXPAND_H
#define QLOG_EXPAND_H

/**
 * qlog expand will force many multi gates to expand if they are found to not
 * be irrelevant. This is to "smoosh" them together with other quantum gates
 * in the shrinking phase. In the long run this should improve runtime when the
 * time comes for it. qlog_expand expects there to be a "checkpoint" for last
 * optimized section of the qlog_node, and will walk through a function table,
 * determining if a certain gate type matches to be expanded, and is not
 * redundant. It will be ideal to have all expanders to be independent of each
 * other, as why would they care who is expanding what, that is for the
 * optimizer to care about.
 *
 * NOTE: The author isnt very appealed with this implementation, this is a
 * simply a basic intro into expanding gates, each gate is equally as annoying
 * to expand as the next. The internals of the API need to be examined again
 * soon.
 */
void qlog_expand_qft(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node);
void qlog_expand_rccx(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node);
void qlog_expand_rc3x(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node);
void qlog_expand_swap(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node);
void qlog_expand_rxx(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node);
void qlog_expand_rzz(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node);
void qlog_expand_ccx(qlog_graph_t *qlog_graph, qlog_node_t *qlog_node);
bool qlog_expand(qlog_graph_t *qlog_graph);

#endif
