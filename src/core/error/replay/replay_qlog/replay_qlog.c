#include "replay_qlog.h"

void replay_qlog_delete(replay_qlog_t *replay_qlog) {
  free(replay_qlog);
  return;
}

replay_qlog_t *replay_qlog_init(qlog_t *qlog, qlog_error_e error_type) {
  replay_qlog_t *replayer = (replay_qlog_t *)malloc(sizeof(replay_qlog_t));

  if (!replayer) {
    return NULL;
  }

  memset(replayer, 0, sizeof(replay_qlog_t));

  replayer->error_code = error_type;
  replayer->qlog = qlog;

  return replayer;
}

bool replay_qlog(void *data, int data_error) {
  if (!data) {
    return false;
  }

  replay_qlog_t *qlog_replay;

  qlog_t *qlog = (qlog_t *)data;
  qlog_error_e error_code = (qlog_error_e)data_error;

  qlog_replay = replay_qlog_init(qlog, error_code);

  if (!qlog_replay) {
    return false;
  }

  replay_qlog_delete(qlog_replay);

  return true;
}
