#include <atomic>
#include <base.h>
#include <mutex>
#include <quack_start.h>
#include <quack_tetris.h>
#include <thread>
#include <vector>

#pragma once
#ifndef QUACK_HIVE_H
#define QUACK_HIVE_H

namespace QuackHive {
typedef std::pair<std::atomic<size_t>, std::atomic<size_t>> hive_pair_t;

void quack_hive_pool_worker_thread(void);

struct QuackHiveWorker {
  QuackHiveWorker()
      : current_pair{}, working_idx{}, thread{quack_hive_pool_worker_thread} {};

  ~QuackHiveWorker() {
    if (thread.joinable()) {
      thread.join();
    }
  }

  std::thread thread;
  QuackTetris::tetris_pair_t current_pair;
  hive_pair_t working_idx;
};

class QuackHivePool {
public:
  QuackHivePool(const QuackHivePool &) = delete;
  QuackHivePool operator=(const QuackHivePool &) = delete;

  QuackHivePool()
      : work_queue{}, work_queue_lock{}, hive_workers{total_hive_threads} {};

  static const QuackHivePool &get() {
    static const QuackHivePool getter{};
    return getter;
  }

  friend void quack_hive_pool_worker_thread(size_t worker_id);

private:
  static constexpr size_t total_hive_threads = 16;
  static_assert(QUBIT_MAX_SIZE % total_hive_threads == 0);

  mutable std::vector<QuackHiveWorker> hive_workers;

  mutable std::vector<QuackTetris::tetris_pair_t> work_queue;
  mutable std::mutex work_queue_lock;

  ~QuackHivePool() = default;
};
}; // namespace QuackHive

#endif
