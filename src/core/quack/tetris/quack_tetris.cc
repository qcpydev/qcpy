#include <quack_tetris.h>

const void
QuackTetris::QuackImporterTetris::insert_entry(qlog_entry_t &qlog_entry,
                                               qubit_t qubit) {
  assert(qlog_entry.qubit_count && qlog_entry.qubit_bitmask & (size_t)1
                                                                  << qubit);
  // list of things to do in this routine:
  // 1) determine if the qubit slot in the current pair can stuff it in A slot
  // 2) if it can stuff it in a slot, get the needed matrix that represents the
  // qubit, then do such matrix multiplication
  // 3) i
}
