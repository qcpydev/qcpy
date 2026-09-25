#include <base_tools.h>
#include <iostream>
#include <quack.h>
#include <quack_import.h>
#include <quack_start.h>

void QuackBase::quack_start_instance(char **argv) {
  QuackStart::QuackBootArgs::get(argv);
  QuackImport::QuackImporter::get();
}
