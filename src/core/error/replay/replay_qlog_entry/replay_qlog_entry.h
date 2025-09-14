#include <qlog_entry.h>
#include <stdint.h>

#ifndef REPLAY_QLOG_ENTRY_H
#define REPLAY_QLOG_ENTRY_H

typedef struct {
  qlog_entry_t *qlog_entry;
  qlog_entry_error_e error_code;
} replay_qlog_entry_t;

bool replay_qlog_entry(void *data, int error_type);
replay_qlog_entry_t *replay_qlog_entry_init(qlog_entry_t *replay_qlog_entry,
                                            qlog_entry_error_e error_type);
void replay_qlog_entry_delete(replay_qlog_entry_t *replay);

#endif
