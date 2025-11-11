#include <importer.h>
#include <stdint.h>

#pragma once
#ifndef ERROR_IMPORTER_H
#define ERROR_IMPORTER_H

/*
 * Error handler for importer section of code, also handles the import_sorted,
 * as it just makes sense.
 * */

typedef struct {
  import_sorted_error_e error_code;
  import_sorted_t *import_sorted;
} error_import_sorted_t;

typedef struct {
  importer_error_e error_code;
  import_t *importer;
} error_importer_t;

bool error_import_sorted(void *data, int error_type);
error_import_sorted_t *
error_import_sorted_init(import_sorted_t *import_sorted,
                         import_sorted_error_e error_type);
void error_import_sorted_delete(error_import_sorted_t *error_import_sorted);

bool error_importer(void *data, int error_type);
error_importer_t *error_importer_init(import_t *importer,
                                      importer_error_e error_type);
void error_importer_delete(error_importer_t *error_importer);
#endif
