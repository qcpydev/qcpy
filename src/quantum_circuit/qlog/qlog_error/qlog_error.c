#include "qlog_error.h"

void qlog_error_set_type(struct qlog_error_def* qlog_error, void* value, qlog_error_val_e type) {
  if (!qlog_error || !value) {
    return;
  }

  qlog_error->qlog_error_type->qlog_error_type = type;

  switch(type) {
    case(QLOG_ERROR_VAL):
      qlog_error->qlog_error_type->qlog_error_type_value.qlog_error_parent = (struct qlog_def*) value;
      break; 
    case(QLOG_ENTRY_ERROR_VAL):
      qlog_error->qlog_error_type->qlog_error_type_value.qlog_error_entry_parent = (struct qlog_entry_def*) value;
      break; 
    default:
      break;
  }
}

struct qlog_error_def* qlog_error_init(void* content, const char* failed_func, const char* reason, qlog_error_val_e type) {
  if (!content) {
    return NULL;
  }

  struct qlog_error_def* qlog_error = (struct qlog_error_def*) malloc(sizeof(struct qlog_error_def));
  qlog_error_set_type(qlog_error, content, type);

  return qlog_error;
}

void qlog_error_cleanup(struct qlog_error_def* qlog_error) {
  if (!qlog_error) {
    return;
  }
  if (!qlog_error->qlog_error_type) {
    free(qlog_error->qlog_error_type);
    qlog_error->qlog_error_type = NULL;
  }

  free(qlog_error);
  qlog_error = NULL;
}

void qlog_error_dump_content(struct qlog_error_def* qlog_error) {
  if (!qlog_error) {
    return;
  }

  bool verbose = true;

  struct qlog_error_type_def* error_type = qlog_error->qlog_error_type;

  switch(qlog_error->qlog_error_type->qlog_error_type) {
    case(QLOG_ERROR_VAL):
      qlog_dump_content(error_type->qlog_error_type_value.qlog_error_parent, verbose); 
      break; 
    case(QLOG_ENTRY_ERROR_VAL):
      qlog_entry_dump_content(error_type->qlog_error_type_value.qlog_error_entry_parent, verbose); 
      break; 
    case(QLOG_REGISTER_ERROR_VAL):
      qlog_register_dump_log(verbose); 
      break; 
  }
}

void qlog_error(const char* qlog_error_failed_func, bool is_warning, const char* reason, void* type_value, qlog_error_val_e type) {
  if (!type_value) {
    printf("qlog_error: no passed in value found to find issue\n");
    exit(EXIT_FAILURE);
  }

  if (!qlog_error_failed_func) {
    printf("qlog_error: no function name found\n");
  }
  else {
    printf("qlog failed at: %s, dumping content now\n", qlog_error_failed_func);
  }
  
  if (!reason) {
    reason = "Unknown";
  }

  qlog_error_def* qlog_error_set = qlog_error_init(type_value, qlog_error_failed_func, reason, type);

  bool verbose = true;

  printf("Please submit an issue at the maintained repository here: github.com/qcpydev/qcpy\n");
  printf("ticket name: qlog: %s, %s ->, %s\n", is_warning ? "warning" : "error" , qlog_error_failed_func, reason);

  if (!is_warning) {
    qlog_error_dump_content(qlog_error_set);
    exit(EXIT_FAILURE);
  }

  qlog_error_cleanup(qlog_error_set);
}

