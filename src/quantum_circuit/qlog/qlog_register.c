
#include "qlog.h"
#include "qlog_error/qlog_error.h"
#include "qlog_register.h"


#define MAX_QLOG_REGISTER_SIZE (UINT8_MAX)
struct qlog_register_def* qlog_register;
bool is_qlog_register_enabled = false;

void qlog_register_init(void) {
  qlog_register = (struct qlog_register_def*) malloc(sizeof(struct qlog_register_def*));
  qlog_register->qlog_register_qlogs = (struct qlog_def**) malloc(sizeof(struct qlog_def*) * MAX_QLOG_REGISTER_SIZE);
  qlog_register->qlog_register_size = 0;
  if (!qlog_register) {
    QLOG_REGISTER_SET_ERROR(qlog_register, "qlog register malloc failed", QLOG_ERROR);
  }
  is_qlog_register_enabled = true;
}

qlog_register_append_res qlog_register_append(struct qlog_def* qlog) {
  if (!is_qlog_register_enabled) {
    qlog_register_init();
  }

  if (!qlog_register) {
    QLOG_REGISTER_SET_ERROR(qlog_register, "qlog register is null", QLOG_ERROR);
  }

  if (qlog_register->qlog_register_size == MAX_QLOG_REGISTER_SIZE) {
    QLOG_REGISTER_SET_ERROR(qlog_register, "qlog_register is full", QLOG_ERROR);
  }

  qlog_register->qlog_register_qlogs[qlog_register->qlog_register_size] = qlog; 
  ++qlog_register->qlog_register_size;

  return QLOG_REGISTER_APPEND_SUCCESS;
}

qlog_register_delete_res qlog_register_delete(void) {
  free(qlog_register->qlog_register_qlogs); 
  qlog_register->qlog_register_qlogs = NULL;
  free(qlog_register);
  qlog_register = NULL;
  return QLOG_REGISTER_DELETE_SUCCESS;
}

void qlog_register_dump_log(bool verbose) {
  if (!is_qlog_register_enabled) {
    QLOG_REGISTER_SET_ERROR(qlog_register, "qlog register is not set", QLOG_WARNING);
    return;
  }

  printf("Qlog Register Dump Log\n Qlog Size: %d", qlog_register->qlog_register_size);
  for (uint16_t i = 0; i < qlog_register->qlog_register_size; ++i) {
    qlog_dump_content(qlog_register->qlog_register_qlogs[i], verbose);
  }
  printf("\n========================\n");
}
