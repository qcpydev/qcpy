#include <importer.h>
#include <qlog_scheduler.h>
#include <qlog_thread_pool.h>

#pragma once
#ifndef QLOG_INFRA_H
#define QLOG_INFRA_H

/*
 * qlog infra is a wrapper to organize the scheduler and thread pool. infra will
 * await for access to the import port to gain access to sorted items. It
 * will call the scheduler when given access to weight which threads should run,
 * and rely on the thread pool to create entries on qlog, clog, etc. and finally
 * signal to compress threads to async work on the unprocessed segments of the
 * qlog.
 * Use qlog_infra_init to set up the thread pool/scheduler (weight distribution)
 * Use qlog_infra_process, which will await work to be done in the sorted entries
 */

void qlog_infra_init();
void qlog_infra_schedule();
void qlog_infra_handler();
void qlog_infra_process();

#endif
