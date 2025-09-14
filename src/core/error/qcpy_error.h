#include <stdbool.h>

#ifndef QCPY_ERROR_H
#define QCPY_ERROR_H

#define WARNING_FLAG 0x1
#define DEBUG_FLAG 0x2

#define QCPY_ERROR(type, data, data_error)                                     \
  (qcpy_error(type, data, data_error, ~(WARNING_FLAG | DEBUG_FLAG), __func__))

#define QCPY_DEBUG_ERROR(type, data, data_error)                               \
  (qcpy_error(type, data, data_error, ~(WARNING_FLAG), __func__))

#define QCPY_WARN(type, data, data_error)                                      \
  (qcpy_error(type, data, data_error, WARNING_FLAG, __func__))

#define QCPY_DEBUG_WARN(type, data, data_error)                                \
  (qcpy_error(type, data, data_error, (WARNING_FLAG | DEBUG_FLAG), __func__))

typedef enum {
  QCPY_ERROR_QLOG,
  QCPY_ERROR_QLOG_REGISTER,
  QCPY_ERROR_QLOG_ENTRY,
} qcpy_error_e;

typedef struct {
  qcpy_error_e error_type;
  const char *error_location;
  const char *error_info;
  const char *error_title;
  const char *error_description;
  const char *error_labels;
  int error_flags;
} qcpy_error_t;

void qcpy_error(qcpy_error_e type, void *data, int data_error, int error_flags,
                const char *error_location);
void qcpy_error_create_issue(qcpy_error_t *qcpy_error);
void qcpy_error_delete(qcpy_error_t *qcpy_error);

#endif
