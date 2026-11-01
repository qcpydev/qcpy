#include <atomic>
#include <block.h>
#include <qlog.h>
#include <quack_start.h>
#include <shared_mutex>
#include <vector>

#pragma once
#ifndef QUACK_TETRIS_H
#define QUACK_TETRIS_H

namespace QuackTetris {
typedef std::pair<QuackStart::QuackUnitaryMatrix,
                  QuackStart::QuackUnitaryMatrix>
    tetris_pair_t;

class QuackImporterTetris {
public:
  QuackImporterTetris(const QuackImporterTetris &) = delete;
  QuackImporterTetris operator=(const QuackImporterTetris &) = delete;

  QuackImporterTetris() : tetrify_matrix(QUBIT_MAX_SIZE), lock{} {};

  static const QuackImporterTetris &get() {
    static QuackImporterTetris obj;
    return obj;
  }

  void insert_entry(qlog_entry_t &qlog_entry, qubit_t qubit,
                    bitmask_t qubits_used, qlog_entry_id_t last_entry) const;
  mutable std::shared_mutex lock;

  mutable std::pair<std::atomic<bitmask_t>, std::atomic<bitmask_t>>
      pair_bitmask;

  mutable std::vector<tetris_pair_t> tetrify_matrix;
  size_t send_off_work_counter;
  size_t send_off_work_sloppy_counter;

private:
  void send_off_work() const;
  void reset_current_pair() const;
  void insert_entry_pick_pair_slot(qlog_entry_t &qlog_entry,
                                   qubit_t qubit) const;

  ~QuackImporterTetris() = default;
};

}; // namespace QuackTetris

#endif
