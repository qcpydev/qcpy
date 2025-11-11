#pragma once

#ifndef BLOCK_H
#define BLOCK_H
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>

/*
 * blocks are used to communicate between the python API and the backend system.
 * the goal is to create a universal space to accumulate entries / requests.
 * Eventually, it is needed to have blocks have "weights" where we can determine
 * when we need to flush to the backend / determine what needs to get flushed.
 */

#define IMPORT_MAX_SIZE (uint64_t)16


typedef enum {
  IMPORTER_QLOG_ENTRY,
  IMPORTER_QLOG_CREATE,
  IMPORTER_QLOG_DELETE,
} block_type_e;

typedef union {
  uint64_t qlog_register;
  uint64_t gate_type;
  uint64_t gate_name;
  uint64_t qubits;
  uint64_t qubit_cnt;
  float theta;
  float phi;
  float lambda;
} block_content_u;

typedef struct block_s {
  block_type_e type;
  block_content_u content;
} block_t;

typedef struct block_port_s {
  pthread_mutex_t lock;
  block_t queue[IMPORT_MAX_SIZE];
  uint64_t size;
} block_port_t;

port_t *port = NULL;

bool validate_block(block_t* block);

// get block weight down here
#endif // BLOCK_H
