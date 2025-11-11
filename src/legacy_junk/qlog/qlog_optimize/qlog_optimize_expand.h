#include <stdio.h>
#include "../qlog_entry.h"
#include "../qlog.h"
#include "qlog_optimize.h"

/*
 * qlog optimize expand
 * Atomic, independent series of functions that are used to expand qlog optimize
 * list of current items.
 * These can trigger specifically when a qlog optimize shrink has determined
 * that a gate needs to be expanded for future shrink work to find optimizations.
 * This is so that next iterations of cleaning up next phases of shrinking
 * the qlog_optimize list of work that needs to be sent to quack.
 */

#pragma once
#ifndef QLOG_OPTIMIZE_EXPAND_H
#define QLOG_OPTIMIZE_EXPAND_H

void qlog_optimize_expand_ccx(struct qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_qft(struct qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_rccx(struct qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_rc3x(struct qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_rxx(struct qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_rzz(struct qlog_optimize_def* qlog_optimize);
void qlog_optimize_expand_swap(struct qlog_optimize_def* qlog_optimize);

#endif // QLOG_OPTIMIZE_EXPAND_H
