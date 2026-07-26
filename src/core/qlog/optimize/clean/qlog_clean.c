#include <assert.h>
#include <base.h>
#include <qlog_clean.h>

bool qlog_clean(qlog_graph_t *qlog_graph) {
  uint16_t *controlled = base_decompress_qubit_bitpack(0, 0);
  if (qlog_graph) {
  }
  assert(qlog_graph);
  return true;
}
