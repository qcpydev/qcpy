#include <exporter_migrate.h>
#include <quack_import.h>

void QuackImport::quack_importer_worker_thread(size_t start_range) {
  const auto &quack_importer = QuackImporter::QuackImporter::get();
  const auto &quack_tetris = QuackTetris::QuackImporterTetris::get();
  exporter_migrate_t *export_migrate = quack_importer.get_exporter();
  assert(export_migrate);

  qlog_entry_t qlog_entry{};

  size_t end_range = quack_importer.get_thread_end_range(start_range);

  while (1) {
    for (auto qubit{start_range}; qubit <= end_range; ++qubit) {
      ::exporter_migrate_dequeue_single(export_migrate, qubit, &qlog_entry);

      if (qlog_entry.qubit_count != 0) {
        quack_tetris.insert_entry(qlog_entry, qubit);
      }
    }
  }
}
