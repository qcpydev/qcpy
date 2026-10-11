#include <thread>

namespace QuackExport {

void quack_exporter_worker_thread();
class QuackExporter {
public:
  QuackExporter(const QuackExporter &) = delete;
  QuackExporter operator=(const QuackExporter &) = delete;

  QuackExporter() : is_ready{false}, thread{quack_exporter_worker_thread} {};

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

private:
  bool is_ready;
  std::thread thread;
};
}; // namespace QuackExport
