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
#include <block.h>

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
 * importer will then inform the port that all information has been
 * successfully sent over to qlog, and can zero out the shared memory for more
 * requests.
 */

#define IMPORT_MAX_SIZE (uint64_t)16

typedef enum {
  IMPORT_ERROR_NULL,
} importer_error_e;

typedef enum {
  IMPORT_SORTED_ERROR_NULL,
} import_sorted_error_e;

typedef struct import_s {
  sem_t empty;
  sem_t full;
  block_t queue[IMPORT_MAX_SIZE];
  uint64_t size;
} import_t;

typedef struct imported_block_s imported_block_t;
struct imported_block_s {
  block_t* block;
  imported_block_t* next_block;
};

typedef struct import_sorted_s {
  pthread_mutex_t sorting;
  uint64_t count;
  pthread_mutex_t registers_lock[IMPORT_MAX_SIZE];
  imported_block_t* registers[IMPORT_MAX_SIZE];
  imported_block_t* registers_last[IMPORT_MAX_SIZE];
} importer_t;

extern import_t* import_queue;
extern importer_t import_sorted;

void port_import_init();
void port_import_enqueue(block_t* block);
imported_block_t* port_imported_init(block_t* reg);

void port_imported_clear();
uint64_t port_import_handler();
void port_import_clear();

uint64_t port_import_sort(block_t* cxt);
void port_import_enqueue(block_t* ctx);

#endif
