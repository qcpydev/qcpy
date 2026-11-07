#include <qlog_print.h>
#include <stdlib.h>

void qlog_print_init(qlog_print_t *qlog_print, qubit_t qubits) {
  assert(qlog_print);

  qlog_print->count = calloc(qubits, sizeof(size_t));

  assert(qlog_print->count);
}
void qlog_print_append(qlog_print_t *qlog_print, bitmask_t qubit_bitmask,
                       bitmask_t target_bitmask, base_gate_e gate_name) {
  assert(qubit_bitmask && qlog_print);
  qlog_print_item_t *qlog_print_item =
      qlog_print_item_init(qubit_bitmask, target_bitmask, gate_name);

  if (!qlog_print->list) {
    qlog_print->list = qlog_print_item;
    qlog_print->last = qlog_print_item;
  } else {
    qlog_print->last->next = qlog_print_item;
    qlog_print->last = qlog_print->last->next;
  }

  qlog_print->total_count++;
}

void qlog_print_delete(qlog_print_t *qlog_print);
void qlog_printer(qlog_print_t *qlog_print);

qlog_print_item_t *qlog_print_item_init(bitmask_t qubit_bitmask,
                                        bitmask_t target_bitmask,
                                        base_gate_e gate_name) {
  qlog_print_item_t *qlog_print_item =
      (qlog_print_item_t *)calloc(1, sizeof(qlog_print_item_t));

  qlog_print_item->qubit_bitmask = qubit_bitmask;
  qlog_print_item->target_bitmask = target_bitmask;
  qlog_print_item->gate_name = gate_name;

  return qlog_print_item;
}

void qlog_printer_item(qlog_print_item_t *qlog_print_item) {}
