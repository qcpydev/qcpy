#include <mutex>
#include <quack_tetris.h>

void QuackTetris::QuackImporterTetris::reset_current_pair() const {}

void QuackTetris::QuackImporterTetris::send_off_work() const {
  std::unique_lock<std::shared_mutex> _unique_lock(lock);

  if (send_off_work_counter != send_off_work_sloppy_counter) {
    return;
  }
}

void QuackTetris::QuackImporterTetris::insert_entry_pick_pair_slot(
    qlog_entry_t &qlog_entry, qubit_t qubit) const {
  std::shared_lock<std::shared_mutex> _shared_lock(lock);
  QuackStart::QuackUnitaryMatrix matrix{qlog_entry};

  tetrify_matrix[qubit].first *= matrix;
}

void QuackTetris::QuackImporterTetris::insert_entry(
    qlog_entry_t &qlog_entry, qubit_t qubit, bitmask_t qubits_used,
    qlog_entry_id_t last_entry) const {
  assert(qlog_entry.qubit_count && qlog_entry.qubit_bitmask & (size_t)1
                                                                  << qubit);

  const auto is_tetris_full = [this, qubits_used, qlog_entry,
                               last_entry]() -> bool {
    return (this->pair_bitmask.first.load() &
            this->pair_bitmask.second.load()) == qubits_used ||
           qlog_entry.entry_id == last_entry;
  };

  insert_entry_pick_pair_slot(qlog_entry, qubit);

  if (is_tetris_full()) {
    send_off_work();
  }
}
