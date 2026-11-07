#include <base.h>
#include <stdio.h>
#include <string.h>

typedef struct qlog_print_item_t qlog_print_item_t;

struct qlog_print_item_t {
  qlog_print_item_t *next;
  bitmask_t qubit_bitmask;
  bitmask_t target_bitmask;
  base_gate_e gate_name;
};
typedef struct qlog_print_s {
  size_t *count;
  size_t total_count;
  qlog_print_item_t *list;
  qlog_print_item_t *last;
} qlog_print_t;

void qlog_print_append(qlog_print_t *qlog_print, bitmask_t qubit_bitmask,
                       bitmask_t target_bitmask, base_gate_e gate_name);

void qlog_print_delete(qlog_print_t *qlog_print);
void qlog_printer(qlog_print_t *qlog_print);

qlog_print_item_t *qlog_print_item_init(bitmask_t qubit_bitmask,
                                        bitmask_t target_bitmask,
                                        base_gate_e gate_name);

void qlog_printer_item(qlog_print_item_t *qlog_print_item);

void qlog_print_init(qlog_print_t *qlog_print, qubit_t qubits);
