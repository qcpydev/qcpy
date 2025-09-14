#include <qlog.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#pragma once
#ifndef QLOG_REGISTER_H
#define QLOG_REGISTER_H

/*
 * qlog register is designed at organized references to qlogs in a defined place
 * this is used to insert qlog entries into designated qlogs, as well as
 * organize when a qlog needs to send it's data over to quack, or other parts of
 * the ecosystem.
 */
#define QLOG_BUFFER_SIZE 256

typedef enum {
  QLOG_REGISTER_NULL,
} qlog_register_error_e;

typedef struct qlog_register_s qlog_register_t;
struct qlog_register_s {
  qlog_t *qlog;
};

typedef struct qlog_buffer_s {
  qlog_register_t registers[QLOG_BUFFER_SIZE];
  uint64_t size;
} qlog_buffer_t;

#endif // !QLOG_REGISTER_H
