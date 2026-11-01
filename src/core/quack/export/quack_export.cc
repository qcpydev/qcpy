#include <quack_export.h>

void QuackExport::quack_exporter_worker_thread() {
  auto &quack_exporter = QuackExport::QuackExporter::get();

  for (;;) {
    std::unique_lock<std::mutex> lock(quack_exporter.lock);
    quack_exporter.get_cond().wait(
        lock, [&quack_exporter] { return quack_exporter.export_work_items(); });
  }
}
