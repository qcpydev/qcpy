#include <block.h>
#include <qlog.h>
#include <stdbool.h>
#include <stdint.h>

#pragma once
#ifndef QLOG_REGISTER_H
#define QLOG_REGISTER_H

/*
 * qlog register is designed at organized references to qlogs in a defined place
 * this is used to insert qlog entries into designated qlogs, as well as
 * organize when a qlog needs to send it's data over to quack, or other parts of
 * the ecosystem.
 */

#define QLOG_REGISTER_BUF_SIZE 16

typedef enum
{
    QLOG_REGISTER_NULL,
} qlog_register_error_e;

typedef struct qlog_register_s qlog_register_t;
struct qlog_register_s
{
    qlog_t* qlog;
    uint64_t id;
};

typedef struct qlog_register_buf_s qlog_register_buf_t;
struct qlog_register_buf_s
{
    qlog_register_t reg;
    qlog_register_buf_t* next;
    qlog_register_buf_t* last;
};

qlog_register_buf_t* qlog_register_buf_init(qlog_register_buf_t* qlog_buffer);
void qlog_register_buf_delete(qlog_register_buf_t* qlog_register_buf);
void qlog_register_delete(qlog_register_t* qlog_register);
void qlog_register_add(qlog_register_buf_t* qlog_register_buf, block_t block);

bool qlog_register_buf_check_id(qlog_register_buf_t* qlog_register_buf, uint64_t id);
qlog_register_buf_t* qlog_register_buf_find(qlog_register_buf_t* qlog_buffer, uint64_t id);

#endif // !QLOG_REGISTER_H
