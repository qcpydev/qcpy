#include <qlog_graph.h>
#include <qlog_node.h>
#include <qlog_entry.h>

#pragma once
#ifndef QLOG_OPTIMIZE_H
#define QLOG_OPTIMIZE_H

/*
 * qlog optimize will walk through a qlog graph and find patterns that can be reduced
 * or find gates that can be expanded to then be optimized, this will change the qlog graph
 * as a whole. this is the parent optimize/expand code for qlog, and will rely on
 * subcomponents to do this work. This is only a space to wrap everything together nicely
 *
 * Some obvious todos:
 * find a checkpoint spot where we know that we can assume that everything has been optimized
 * perhaps determine what has ran?
 */

bool qlog_optimize(qlog_graph_t* qlog_graph, bool force);

#endif
