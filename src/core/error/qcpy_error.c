#include <qcpy_error.h>
#include <stdio.h>
#include <stdlib.h>

typedef bool (*qcpy_error_replay)(void *data, int data_error);
typedef void (*qcpy_error_del)(qcpy_error_t *);

void qcpy_error_create_issue(qcpy_error_t *qcpy_error) {
  if (!qcpy_error) {
    return;
  }

  const char *title = qcpy_error->error_title;
  const char *error_location = qcpy_error->error_location;
  const char *error_info = qcpy_error->error_info;
  const char *error_description = qcpy_error->error_info;
  const char *debug =
      qcpy_error->error_flags & DEBUG_FLAG ? "[DEBUG MODE]" : "";
  const char *warning =
      qcpy_error->error_flags & WARNING_FLAG ? "[WARNING]" : "";
  const char *labels = qcpy_error->error_labels;

  printf("\n================================================\n");
  printf("[Auto Generated Issue]: %s %s %s: %s -> %s\n", title, debug, warning,
         error_location, error_info);
  printf("Labels: %s", labels);
  printf("Description: %s\n", "");
  printf("\n================================================\n");
  printf("%s", error_description);
}

void qcpy_error_delete(qcpy_error_t *qcpy_error) {
  if (!qcpy_error) {
    return;
  }

  free(qcpy_error);
  qcpy_error = NULL;
}
