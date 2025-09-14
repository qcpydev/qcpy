#include <pthread.h>
#include <importer.h>
#include <qlog_register.h>
#include <stdint.h>

#pragma once
#ifndef QLOG_SCHEDULER_H
#define QLOG_SCHEDULER_H

/*
 * qlog scheduler is apart of qlog infra. It is designed to schedule and
 * prioritize threads apart of qlog threadpool. The scheduler will also set the
 * range of work each thread will handle based on hit entries in the sorted queues.
 * If the user is expecting a front end request from the library, we expect to
 * flush everything only for that specific qlog, so only one thread will be
 * running in qlog thread pool.
 * */
void qlog_scheduler_init();

#endif /* QLOG_SCHEDULER_H  */
