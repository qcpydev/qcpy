#include <assert.h>
#include <block.h>
#include <stdbool.h>
#include <stdint.h>

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

typedef enum
{
    IMPORT_ERROR_NULL,
} importer_error_e;

typedef enum
{
    IMPORT_SORTED_ERROR_NULL,
} import_sorted_error_e;

typedef struct import_block_s import_block_t;
struct import_block_s
{
    block_t block;
    import_block_t* next;
};

typedef struct import_s
{
    pthread_mutex_t lock;
    uint64_t count;
    pthread_mutex_t queue_lock[IMPORT_MAX_SIZE];
    import_block_t* queue[IMPORT_MAX_SIZE];
    import_block_t* queue_last[IMPORT_MAX_SIZE];
} import_t;



void importer_init();
void importer_append(block_t block);
void importer_clear();
void importer_delete_queue(uint64_t idx);
void importer_sort_ported(port_t* port);

import_block_t* import_block_init();
void import_block_delete(import_block_t* import_block);

extern import_t importer;

#endif
