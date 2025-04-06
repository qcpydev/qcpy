#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

extern bool is_qlog_register_enabled;

typedef struct qlog_register_def {
  struct qlog_def** qlog_register_qlogs;
  uint16_t qlog_register_size;
} qlog_register_def;

typedef enum qlog_register_delete_res {
  QLOG_REGISTER_DELETE_SUCCESS,
  QLOG_REGISTER_DELETE_ERROR_NOT_NULL,
  QLOG_REGISTER_DELETE_ERROR_UNKNOWN,
} qlog_register_delete_res;

typedef enum qlog_register_append_res {
  QLOG_REGISTER_APPEND_SUCCESS,
  QLOG_REGISTER_APPEND_ERROR_FULL,
  QLOG_REGISTER_APPEND_ERROR_NULL,
  QLOG_REGISTER_APPEND_ERROR_UNKNOWN,
} qlog_register_append_res;

void qlog_register_init(void);
enum qlog_register_delete_res qlog_register_delete(void);
void qlog_register_dump_log(bool verbose);
qlog_register_append_res qlog_register_append(struct qlog_def* qlog);
