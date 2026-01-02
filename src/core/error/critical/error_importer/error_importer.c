#include <error_importer.h>
#include <importer.h>
/*
bool error_import_sorted(void *data, int error_type) {
  error_import_sorted_t *error_import_sorted;

  import_sorted_t *import_sorted_ptr = (import_sorted_t *)data;
  import_sorted_error_e error_code = (import_sorted_error_e)error_type;

  error_import_sorted = error_import_sorted_init(import_sorted_ptr, error_code);

  if (!error_import_sorted) {
    return false;
  }

  error_import_sorted_delete(error_import_sorted);

  return true;
}

error_import_sorted_t *
error_import_sorted_init(import_sorted_t *import_sorted,
                         import_sorted_error_e error_type) {
  error_import_sorted_t *error_import_sorted;
  error_import_sorted =
      (error_import_sorted_t *)malloc(sizeof(error_import_sorted_t));

  if (!error_import_sorted) {
    return NULL;
  }

  error_import_sorted->import_sorted = import_sorted;
  error_import_sorted->error_code = error_type;

  return error_import_sorted;
}

void error_import_sorted_delete(error_import_sorted_t *error_import_sorted) {
  if (!error_import_sorted) {
    return;
  }

  free(error_import_sorted);
  error_import_sorted = NULL;
}

bool error_importer(void *data, int error_type) {
  error_importer_t *error_importer;

  import_t *importer_ptr = (import_t *)data;
  importer_error_e error_code = (importer_error_e)error_type;

  error_importer = error_importer_init(importer_ptr, error_code);

  if (!error_importer) {
    return false;
  }

  error_importer_delete(error_importer);

  return true;
}

error_importer_t *error_importer_init(import_t *importer,
                                      importer_error_e error_type) {
  error_importer_t *error_importer =
      (error_importer_t *)malloc(sizeof(error_importer_t));

  if (!error_importer) {
    return NULL;
  }
  error_importer->importer = importer;
  error_importer->error_code = error_type;

  return error_importer;
}

void error_importer_delete(error_importer_t *error_importer) {
  if (!error_importer) {
    return;
  }

  free(error_importer);
}
*/
