#include <qlog_optimize.h>
#include <qlog_clean.h>
#include <qlog_expand.h>
#include <assert.h>

bool qlog_optimize(qlog_graph_t* qlog_graph, bool force) {
  assert(qlog_graph);

  if (force) {

  }

  if (qlog_expand(qlog_graph)) {

  }

  // handle checkpoint
  return qlog_clean(qlog_graph);
}
