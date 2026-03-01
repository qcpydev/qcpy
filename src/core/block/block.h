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

#define QCPY_IMPORT "/qcpy_import"
#define PORT_IMPORT_SEM "/port_import_sem"
#define DOCK_IMPORT_SEM "/dock_import_sem"

#define QCPY_EXPORT "/qcpy_export"
#define PORT_EXPORT_SEM "/port_export_sem"
#define DOCK_EXPORT_SEM "/dock_export_sem"

#define OFLAG_SHARED_MEM_ARGS O_CREAT | O_RDWR
#define MODE_SHARED_MEM 0666
#define OFLAG_SHARED_SEM_ARGS O_CREAT | O_EXCL
#define PROT_ARGS PROT_READ | PROT_WRITE

typedef enum { BLOCK_QLOG_ENTRY, BLOCK_CLOG_ENTRY } block_type_e;

#ifdef __cplusplus
#include <complex>
typedef std::complex<float> float_comp;
#else
#include <complex.h>
typedef float complex float_comp;
#endif

typedef struct block_s {
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
  bool used;
} block_t;

typedef struct import_s {
  block_t queue[IMPORT_MAX_SIZE];
  uint64_t flush_reg;
  int dock_idx;
  int port_idx;
  bool flushing;
  bool ready;
} import_t;

typedef struct export_s {
  float_comp queue[IMPORT_MAX_SIZE];
  uint64_t reg;
  int dock_idx;
  int port_idx;
  bool flushing;
  bool entries_ready;
  bool qcpy_core_ready;
  bool quack_core_ready;
} export_t;

bool validate_block(block_t *block);
void block_add(block_t *block, import_t *port);

extern import_t *importer;
extern export_t *exporter;
extern sem_t *dock_import_sem;
extern sem_t *port_import_sem;

extern sem_t *dock_export_sem;
extern sem_t *port_export_sem;

extern int shared_import_space;
extern int shared_export_space;

#endif // BLOCK_H
