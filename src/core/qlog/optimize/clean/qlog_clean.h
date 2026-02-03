#include <qlog_graph.h>

#pragma once
#ifndef QLOG_CLEAN_H
#define QLOG_CLEAN_H
/*
 * qlog clean will run on qlog graph and only after we run qlog expand.
 * The goal of qlog clean is to remove any redundant work that may happen on the
 * quantum circuit inside of quack/any other matrix cruncher. The list of optimizations
 * will be ever expanding, but should never leave this subcomponent of qlog optimize.
 * NOTE: Yes, this is identical to qlog_expand, lets keep these seperate to reduce
 * headaches.
 *
 */

bool qlog_clean(qlog_graph_t* qlog_graph);

#endif
