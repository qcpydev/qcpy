#pragma once

#ifndef BLOCK_H
#define BLOCK_H
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

// This all is going to go besides shared_mem
#define IMPORT_MAX_SIZE (uint64_t)64

#define IMPORTER_FUNNEL 16u

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

/*
 * blocks are used to communicate between the python API and the backend system.
 * the goal is to create a universal space to accumulate entries / requests.
 *
 * block.c is a general API to handle the enqueueing and dequeueing on the
 * lock free queues. block_t is a struct that is used to store the data needed
 * for a quantum gate to be interpreted by the backend of qcpy. qcpy_connect
 * is the producer, creating blocks in python heap and sending off data to
 * queue. qcpy_core picks up the work and determines the best fit for what
 * to do with it.
 *
 * qcpy_connect will dispatch a unique flush message to grab the data from
 * another queue of some sort. There will be a few instances the author can
 * think about at this time, such as printing info of a given quantum circuit,
 * or the quantum state.
 *
 * This component is responsible for the infra and tooling of queues, the
 * consumer thread creation and handling, and much more. Block queue thread
 * consumers must create themselves with the block_queue_thread_t type and pass
 * in block_queue_thread_info_t param to validate opening and reading the
 * appropriate queue.
 *
 * See block_buffer_queue for more info on the structures that are made for
 * this. This is mainly for general known defined types that are used across
 * qcpy that are associated with blocks.
 */

#define BLOCK_BITMASK_MAX ((bitmask_t)1 << 63)

/* typedefs for block.h  */

/* Used to identify what is in the buffer queue  */
typedef enum : uint8_t {
  BLOCK_QLOG_ENTRY, // qlog (quantum gate) entry
  BLOCK_CLOG_ENTRY, // clog (classical gate) entry
  BLOCK_FLUSH_ENTRY // flush queue signal
} block_type_e;

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

#define BITS_IN_BYTE 8

typedef struct block_s {
  bitmask_t qubit_bitmask;
  bitmask_t controlled_bitmask;
  bitpack_t controlled_bitpack;
  bitmask_t target_bitmask;
  bitpack_t target_bitpack;
  param_t theta;
  param_t phi;
  param_t lmbda;
  reg_t reg : (sizeof(reg_t) * BITS_IN_BYTE);
  gate_t gate : (sizeof(gate_t) * BITS_IN_BYTE);
  qubit_t qubits : (sizeof(qubit_t) * BITS_IN_BYTE);
  qubit_t size : (sizeof(qubit_t) * BITS_IN_BYTE);
  qubit_t controlled_count : (sizeof(qubit_t) * BITS_IN_BYTE);
  qubit_t target_count : (sizeof(qubit_t) * BITS_IN_BYTE);
  block_type_e type : 4;
  bool inverted : 1;
  bool big_endian : 1;
  bool used : 1;
} block_t;

typedef enum {
  QUEUE_STATUS_FULL,
  QUEUE_STATUS_EMPTY,
  QUEUE_STATUS_ENQUEUED,
  QUEUE_STATUS_DEQUEUED
} buffer_queue_status_e;

typedef atomic_size_t block_idx_t;
typedef char dont_use_cache_align_t;
typedef _Atomic(pthread_t) block_queue_tid_t;
typedef size_t block_buffer_queue_id_t;

#define BLOCK_QUEUE_SIZE 8192u
#define BLOCK_QUEUE_IDX_MAX (BLOCK_QUEUE_SIZE - 1u)
#define BLOCK_QUEUE_GET_IDX(idx) ((idx) & (BLOCK_QUEUE_IDX_MAX))
#define CACHE_LINE_SIZE 64

#define BLOCK_QUEUES_TOTAL 16u

typedef struct block_buffer_queue_s {
  block_idx_t head;
  dont_use_cache_align_t _head[CACHE_LINE_SIZE - sizeof(block_idx_t)];

  block_idx_t tail;
  dont_use_cache_align_t _tail[CACHE_LINE_SIZE - sizeof(block_idx_t)];
  block_t queue[BLOCK_QUEUE_SIZE]; // Is 64 byte * 128 entries, so already cache
                                   // aligned

  block_queue_tid_t associated_tid;
  dont_use_cache_align_t
      _associated_tid[CACHE_LINE_SIZE - sizeof(block_queue_tid_t)];
} block_buffer_queue_t;

block_buffer_queue_id_t
block_buffer_queue_init(block_buffer_queue_t *qlog_buffer_queue, pthread_t pid);
void block_buffer_queue_clear(block_buffer_queue_t *qlog_buffer_queue);

/* keeping in struct to expand upon */
typedef struct block_buffer_global_s {
  block_buffer_queue_t queues[BLOCK_QUEUES_TOTAL];

  atomic_size_t total_queues;
  dont_use_cache_align_t _total_queues[CACHE_LINE_SIZE - sizeof(atomic_size_t)];

  atomic_size_t curr_free;
  dont_use_cache_align_t _curr_free[CACHE_LINE_SIZE - sizeof(atomic_size_t)];
} block_buffer_global_t;

void block_enqueue(block_buffer_global_t *block_buffer_global, block_t *block);
block_t block_dequeue(block_buffer_global_t *block_buffer_global,
                      block_buffer_queue_id_t queue_id);

#define BLOCK_BUFFER_QUEUE_GLOBAL_NAME "qcpy_core_block_buffer_queues"

block_buffer_global_t *block_buffer_global_open(int shared_mem_fd);

void block_buffer_global_init(block_buffer_global_t *block_buffer_global);

block_buffer_queue_id_t
block_buffer_queue_assign_tid(block_buffer_global_t *block_buffer_global,
                              pthread_t tid, uint64_t key);

void block_buffer_global_await_setup(
    block_buffer_global_t *block_buffer_global);

// BYE BYE NO MORE
typedef struct import_s {
  block_t queue[IMPORT_MAX_SIZE];
  uint64_t flush_reg;
  int idx;
  bool flushing;
  bool ready;
} import_t;
void block_add(block_t *block, import_t *port);

// BYE BYE NO MORE
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

extern export_t *exporter;
// BYE BYE NO MORE
extern sem_t *dock_export_sem;
extern sem_t *port_export_sem;

#endif // BLOCK_H
