#include <qlog.h>
#include <stdint.h>

#ifndef REPLAY_QLOG_H
#define REPLAY_QLOG_H

typedef struct {
  qlog_t *qlog;
  qlog_error_e error_code;
} replay_qlog_t;

bool replay_qlog(void *data, int data_error);
replay_qlog_t *replay_qlog_init(qlog_t *qlog, qlog_error_e error_code);
void replay_qlog_delete(replay_qlog_t *replay);

#endif
