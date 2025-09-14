#include <assert.h>
#include <replay_qlog_entry.h>
#include <string.h>

const char *replay_qlog_entry_description[] = {
    [QLOG_ENTRY_NULL] = "qlog entry is null",
    [QLOG_ENTRY_NULL_QUBITS] = "qlog entry ",
    [QLOG_ENTRY_FAILED_QUBIT_FLAGS] = "qlog entry failed qubit flags",
};

void replay_qlog_entry_delete(replay_qlog_entry_t *replay) {
  if (!replay) {
    return;
  }

  free(replay);
}

replay_qlog_entry_t *replay_qlog_entry_init(qlog_entry_t *qlog_entry,
                                            qlog_entry_error_e type) {
  if (!qlog_entry) {
    return NULL;
  }

  replay_qlog_entry_t *replayer =
      (replay_qlog_entry_t *)malloc(sizeof(replay_qlog_entry_t));

  if (!replayer) {
    return NULL;
  }

  memset(replayer, 0, sizeof(replay_qlog_entry_t));
  replayer->error_code = type;
  replayer->qlog_entry = qlog_entry;
  return replayer;
}

bool replay_qlog_entry(void *data, int type) {
  if (!data) {
    return false;
  }

  qlog_entry_t *qlog_entry = (qlog_entry_t *)data;
  qlog_entry_error_e fail_type = (qlog_entry_error_e)type;

  replay_qlog_entry_t *entry_replay =
      replay_qlog_entry_init(qlog_entry, fail_type);

  if (!entry_replay) {
    return false;
  }

  return true;
}
