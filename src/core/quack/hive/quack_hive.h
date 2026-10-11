#include <atomic>
#include <base.h>
#include <quack_start.h>
#include <thread>
#include <vector>

namespace QuackHive {

typedef struct quack_hive_entry_s {
  quack_hive_entry_s() {};
  ~quack_hive_entry_s() {};
} quack_hive_entry_t;

void quack_hive_pool_worker_thread(size_t worker_id);

typedef std::pair<std::atomic<size_t>, std::atomic<size_t>> hive_pair_t;

class QuackHivePool {
public:
  QuackHivePool(const QuackHivePool &) = delete;
  QuackHivePool operator=(const QuackHivePool &) = delete;

  QuackHivePool() : working_idx{total_hive_threads} {
    for (size_t i{}; i < total_hive_threads; ++i) {
      threads.emplace_back(quack_hive_pool_worker_thread, i);
    }
  };

  static const QuackHivePool &get() {
    static const QuackHivePool getter{};
    return getter;
  }

  ~QuackHivePool() {
    for (auto &thread : this->threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

  friend void quack_hive_pool_worker_thread(size_t worker_id);

private:
  static constexpr size_t total_hive_threads = 4;
  mutable std::vector<hive_pair_t> working_idx;
  std::vector<std::thread> threads;
};
}; // namespace QuackHive
