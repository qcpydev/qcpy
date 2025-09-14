#include <qcpy_error.h>
#include <qlog.h>
#include <replay_qlog.h>
#include <replay_qlog_entry.h>
#include <replay_qlog_register.h>

#define QCPY_URL "https://github.com/qcpydev/qcpy"

typedef bool (*qcpy_error_replay)(void *data, int data_error);
qcpy_error_replay qcpy_error_replay_type[] = {
    [QCPY_ERROR_QLOG] = replay_qlog,
    //[QCPY_ERROR_QLOG_REGISTER] = replay_qlog_register,
    [QCPY_ERROR_QLOG_ENTRY] = replay_qlog_entry};

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

void qcpy_error(qcpy_error_e type, void *data, int data_error, int error_flags,
                const char *error_location) {
  qcpy_error_t *qcpy_error = (qcpy_error_t *)malloc(sizeof(qcpy_error_t));

  if (!qcpy_error) {
    return;
  }

  memset(qcpy_error, 0, sizeof(qcpy_error_t));

  qcpy_error->error_flags = error_flags;
  qcpy_error->error_location = error_location;
  qcpy_error->error_type = type;

  bool debug_mode = error_flags & WARNING_FLAG;
  bool is_warning = error_flags & DEBUG_FLAG;

  printf("\n%s QCPY %s\n", debug_mode ? "DEBUG" : "",
         is_warning ? "WARNING" : "ERROR");

  if (!is_warning) {
    printf(
        "There has been an internal breaking issue."
        "Attempting to replay up until error.\n"
        "Please file an issue using the generate template to %s after replay\n",
        QCPY_URL);

  } else {
    printf("A warning has been detected, possibly leading to future issues.\n"
           "Please file an issue at: %s\n."
           "Warning Output:\n",
           QCPY_URL);
  }

  bool replay_success = qcpy_error_replay_type[type](data, data_error);

  if (!replay_success) {
    printf("SOMETHING WENT WRONG\n");
    assert(0);
  }

  qcpy_error_create_issue(qcpy_error);
  qcpy_error_delete(qcpy_error);

  if (!is_warning) {
    assert(0);
  }
}
