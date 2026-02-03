#pragma once

#ifndef BLOCK_H
#define BLOCK_H
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * blocks are used to communicate between the python API and the backend system.
 * the goal is to create a universal space to accumulate entries / requests.
 * Eventually, it is needed to have blocks have "weights" where we can determine
 * when we need to flush to the backend / determine what needs to get flushed.
 */

#define IMPORT_MAX_SIZE (uint64_t)64
#define IMPORTER_FUNNEL (uint64_t)4

#define QCPY_PORT "/qcpy_port"
#define PORT_SEM "/port_sem"
#define DOCK_SEM "/dock_sem"

typedef enum
{
    BLOCK_QLOG_ENTRY,
    BLOCK_CLOG_ENTRY
} block_type_e;

typedef struct block_s
{
    block_type_e type;
    uint64_t reg;
    uint64_t qubits;
    uint64_t qubit_bitmask;
    uint64_t controlled_bitmask;
    uint64_t controlled_bitpack;
    uint64_t target_bitmask;
    uint64_t target_bitpack;
    float theta;
    float phi;
    float lmbda;
    int gate;
    uint16_t size;
    uint16_t controlled_count;
    uint16_t target_count;
    bool inverted;
    bool big_endian;
} block_t;

typedef struct port_s
{
    block_t queue[IMPORT_MAX_SIZE];
    uint64_t size;
} port_t;

extern sem_t* dock_sem;
extern sem_t* port_sem;
extern port_t* port;
extern int shared_port_space;

bool validate_block(block_t* block);
void port_add(block_t* block, port_t* port);

// get block weight down here
#endif // BLOCK_H
