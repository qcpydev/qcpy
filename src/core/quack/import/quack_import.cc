#include <exporter_migrate.h>
#include <quack_import.h>

void QuackImport::quack_importer_worker_thread(size_t start_range) {
  const auto &quack_importer = QuackImporter::QuackImporter::get();
  qlog_entry_t qlog_entry{};

  exporter_migrate_t *export_migrate = quack_importer.get_exporter();

  while (1) {
    ::exporter_migrate_dequeue_single(export_migrate, (qubit_t)(start_range),
                                      &qlog_entry);
    std::cout << "QUACKER IS ALIVE!\n";
  }
}
