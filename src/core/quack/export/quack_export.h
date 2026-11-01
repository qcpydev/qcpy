#include <condition_variable>
#include <functional>
#include <map>
#include <mutex>
#include <quack_start.h>
#include <thread>
#include <vector>

namespace QuackExport {
struct QuackExportWorkItem {
  std::vector<std::reference_wrapper<QuackStart::QuackStateVector>>
      state_vectors;
  bitmask_t work_id;
};

void quack_exporter_worker_thread();
class QuackExporter {
public:
  QuackExporter(const QuackExporter &) = delete;
  QuackExporter operator=(const QuackExporter &) = delete;

  QuackExporter() : thread{quack_exporter_worker_thread} {};

  static const QuackExporter &get() {
    static const QuackExporter getter;
    return getter;
  }

  ~QuackExporter() {
    if (thread.joinable()) {
      thread.join();
    }
  }

  friend void quack_exporter_worker_thread();

  void state_vector_add(QuackStart::QuackStateVector &state_vector,
                        bitmask_t work_id) const;

  std::condition_variable &get_cond() const { return cond; }

  inline bool export_work_items() const { return work_items.size(); }

private:
  std::thread thread;
  mutable std::mutex lock;
  mutable std::vector<QuackExportWorkItem> work_items;
  mutable std::map<bitmask_t, size_t> work_item_map;
  mutable std::condition_variable cond;
};
}; // namespace QuackExport
