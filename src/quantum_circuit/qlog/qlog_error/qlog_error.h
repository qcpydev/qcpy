#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../qlog.h"
#include "../qlog_entry.h"
#include "../qlog_optimize/qlog_optimize.h"
#include "../qlog_register.h"

#pragma once
#ifndef QLOG_ERROR_H

/*
 * qlog_error, will take in known structs, and dump the content, while prompting the user 
 * to produce an issue on the repo
 * This will allow easy setting of errors and warnings that were not intended, without simply 
 * setting a null value or relying on the front end only. Theoretically, anything that breaks
 * in qlog is the code's fault, not the user's.
 * All qlog error types need a dump_content function to follow with the qlog_error type
 * All qlog data structures need to have their own respective qlog_errors
 * */

typedef enum {
  QLOG_ERROR,
  QLOG_WARNING,
} qlog_error_e;

typedef enum {
  QLOG_ERROR_VAL,
  QLOG_ENTRY_ERROR_VAL,
  QLOG_REGISTER_ERROR_VAL,
  QLOG_OPTIMIZE_ERROR_VAL,
  QLOG_TRIG_OPT_ERROR_VAL,
  QLOG_OPT_GRAPH_ERROR_VAL,
  QLOG_OPT_SHRINK_ERROR_VAL,
} qlog_error_val_e;

#define QLOG_SET_ERROR(qlog, reason, qlog_error_is_warning) \
  qlog_error(__func__, qlog_error_is_warning == QLOG_WARNING, reason, qlog, QLOG_ERROR_VAL)

#define QLOG_ENTRY_SET_ERROR(qlog_entry, reason, qlog_error_is_warning) \
  qlog_error(__func__, qlog_error_is_warning == QLOG_WARNING, reason, qlog_entry, QLOG_ENTRY_ERROR_VAL)

#define QLOG_REGISTER_SET_ERROR(qlog_register, reason, qlog_error_is_warning) \
  qlog_error(__func__, qlog_error_is_warning == QLOG_WARNING, reason, qlog_register, QLOG_REGISTER_ERROR_VAL)

#define QLOG_OPT_SET_ERROR(qlog_optimize, reason, qlog_error_is_warning) \
  qlog_error(__func__, qlog_error_is_warning == QLOG_WARNING, reason, qlog_optimize, QLOG_OPTIMIZE_ERROR_VAL)

#define QLOG_TRIG_OPT_SET_ERROR(qlog_trigger_optimize, reason, qlog_error_is_warning) \
  qlog_error(__func__, qlog_error_is_warning == QLOG_WARNING, reason, qlog_trigger_optimize, QLOG_TRIG_OPT_ERROR_VAL)

#define QLOG_OPT_GRAPH_SET_ERROR(qlog_optimize_graph, reason, qlog_error_is_warning) \
  qlog_error(__func__, qlog_error_is_warning == QLOG_WARNING, reason, qlog_optimize_graph, QLOG_OPT_GRAPH_ERROR_VAL)

#define QLOG_OPT_SHRINK_SET_ERROR(qlog_optimize_shrink, reason, qlog_error_is_warning) \
  qlog_error(__func__, qlog_error_is_warning == QLOG_WARNING, reason, qlog_optimize_shrink, QLOG_OPT_SHRINK_ERROR_VAL)

typedef struct qlog_error_type_def {
  union {
    struct qlog_def* qlog_error_parent;
    struct qlog_entry_def* qlog_error_entry_parent;
    struct qlog_optimize_def* qlog_error_optimize_parent;
    struct qlog_trigger_optimize_def* qlog_error_trigger_optimize_parent;
    struct qlog_optimize_graph_def* qlog_error_optimize_graph_parent;
  } qlog_error_type_value;
  qlog_error_val_e qlog_error_type;
} qlog_error_type_def;

typedef struct qlog_error_def {
  struct qlog_error_type_def* qlog_error_type;
  union {
    qlog_register_delete_res qlog_error_register_delete_failed;      
    qlog_register_append_res qlog_error_register_append_failed;
    qlog_append_res qlog_error_qlog_delete_failed;
  } qlog_error_e_type;

  const char* qlog_error_reason;
  const char* qlog_error_failed_function;
} qlog_error_def;

void qlog_error(const char* qlog_error_failed_func, bool is_warning, const char* reason, void* type_value, qlog_error_val_e type);
struct qlog_error_def* qlog_error_init(void* const, const char* failed_func, const char* reason,  qlog_error_val_e type);
void qlog_error_cleanup(struct qlog_error_def* qlog_error);
void qlog_error_set_type(struct qlog_error_def* qlog_error, void* value, qlog_error_val_e type);
void qlog_entry_error_cleanup(struct qlog_error_def* qlog_error);
void qlog_error_dump_content(struct qlog_error_def* qlog_error);
#endif // !QLOG_ERROR_H
