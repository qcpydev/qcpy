#pragma once

#ifndef BLOCK_H
#define BLOCK_H
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>

/* blocks are used to communicate between the python API and the backend system.
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

typedef enum : uint8_t { BLOCK_QLOG_ENTRY, BLOCK_CLOG_ENTRY } block_type_e;

#define BLOCK_BITMASK_MAX ((bitmask_t)1 << 63)

#ifdef __cplusplus
#include <complex>
typedef std::complex<float> float_comp;
#else
#include <complex.h>
typedef float complex float_comp;
#endif

typedef uint16_t reg_t;
typedef uint64_t bitmask_t;
typedef uint64_t bitpack_t;
typedef float param_t;
typedef uint8_t qubit_t;
typedef uint8_t gate_t;

typedef struct block_s {
  bitmask_t qubit_bitmask;
  bitmask_t controlled_bitmask;
  bitpack_t controlled_bitpack;
  bitmask_t target_bitmask;
  bitpack_t target_bitpack;
  param_t theta;
  param_t phi;
  param_t lmbda;
  reg_t reg : (sizeof(reg_t) * 8);
  gate_t gate : (sizeof(gate_t) * 8);
  qubit_t qubits : (sizeof(qubit_t) * 8);
  qubit_t size : (sizeof(qubit_t) * 8);
  qubit_t controlled_count : (sizeof(qubit_t) * 8);
  qubit_t target_count : (sizeof(qubit_t) * 8);
  block_type_e type : 4;
  bool inverted : 1;
  bool big_endian : 1;
  bool used : 1;
} block_t;

typedef struct import_s {
  block_t queue[IMPORT_MAX_SIZE];
  uint64_t flush_reg;
  int idx;
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

extern export_t *exporter;

extern sem_t *dock_export_sem;
extern sem_t *port_export_sem;

#endif // BLOCK_H
