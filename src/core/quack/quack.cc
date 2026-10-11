#include <base_tools.h>
#include <quack.h>
#include <quack_export.h>
#include <quack_hive.h>
#include <quack_import.h>
#include <quack_start.h>

void QuackBase::quack_start_instance(char **argv) {
  /* Bootarg grabber for others to read from */
  QuackStart::QuackBootArgs::get(argv);

  /* Used to squeeze together matrices from importer and send off work  */
  QuackTetris::QuackImporterTetris::get();

  /* thread pool to read from qcpy_core */
  QuackImport::QuackImporter::get();

  /* thread pool of workers to crunch matrices */
  QuackHive::QuackHivePool::get();

  /* exporter thread to send off completed state vector work to qcpy_connect */
  QuackExport::QuackExporter::get();
}
