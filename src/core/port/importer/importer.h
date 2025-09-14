#include <assert.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#pragma once
#ifndef IMPORTER_H
#define IMPORTER_H

/*
 * importer is used to clean and organize unsorted writes to circuits from
 * the user. The importer will:
 * - Walk through the new user writes, sorting them into buckets based on
 *   the quantum register ids.
 * - Once sorted and properly queued, the qlog scheduler will pick this up using
 *   an admin thread.
 *
 *   importer will then inform the port that all information has been
 * successfully sent over to qlog, and can zero out the shared memory for more
 * requests.
 */

#define IMPORT_MAX_SIZE 256

typedef enum { IMPORTER_QLOG_ENTRY } import_cxt_type_e;

typedef union {
  uint64_t qlog_register;
  uint64_t gate_type;
  uint64_t gate_name;
  uint64_t qubits;
  uint64_t qubit_cnt;
  float theta;
  float phi;
  float lambda;
} import_cxt_item_u;

typedef struct import_cxt_s import_cxt_t;

struct import_cxt_s {
  import_cxt_item_u item;
  import_cxt_type_e type;
};

typedef struct importer_s {
  sem_t empty;
  sem_t full;
  import_cxt_t queue[IMPORT_MAX_SIZE];
  uint64_t size;
} import_t;

typedef struct imported_item_s imported_item_t;
struct imported_item_s {
  import_cxt_t *cxt;
  imported_item_t *next_cxt;
};

typedef struct import_sorted_s {
  sem_t empty;
  sem_t full;
  uint64_t count;
  imported_item_t *registers[IMPORT_MAX_SIZE];
  imported_item_t *registers_last[IMPORT_MAX_SIZE];
} import_sorted_t;

extern import_t *import_queue;
extern import_sorted_t import_sorted;

void port_import_init();
imported_item_t *port_imported_init(import_cxt_t *reg);
void port_imported_clear();
int port_import_handler();
void port_import_clear();
void port_import_sort(import_cxt_t *cxt);

#endif
