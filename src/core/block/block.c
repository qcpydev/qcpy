#define _GNU_SOURCE
#include <assert.h>
#include <base.h>
#include <base_tools.h>
#include <block.h>
#include <qcpy_error.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#ifdef DEBUG
static void validate_block(block_t *block) {
  assert(block);
  switch (block->type) {
  case (
      BLOCK_QLOG_ENTRY): // check type of block and that it makes sense, break;
  default:
    assert(0);
  }
  return true;
}

static void validate_block_buffer_queue(size_t queue_idx) {
  assert(qlog_buffer_queue);
  // get
}

#endif
static _Atomic size_t counter;

static atomic_size_t
block_buffer_global_new(block_buffer_global_t *block_buffer_global,
                        uint64_t key) {
  assert(block_buffer_global);
  atomic_size_t total = atomic_fetch_add_explicit(
      &block_buffer_global->total_queues, 1, memory_order_relaxed);

  assert(total < BLOCK_QUEUES_TOTAL);

  block_buffer_queue_t *block_buffer_queue = &block_buffer_global->queues[key];

  block_queue_tid_t tid = atomic_load_explicit(
      &block_buffer_queue->associated_tid, memory_order_acquire);

  assert(tid == 0);

  atomic_store_explicit(&block_buffer_queue->associated_tid, pthread_self(),
                        memory_order_release);

  atomic_store_explicit(&block_buffer_queue->tail, 0, memory_order_relaxed);
  atomic_store_explicit(&block_buffer_queue->head, 0, memory_order_relaxed);

  if (!atomic_is_lock_free(&block_buffer_queue->head)) {
    fprintf(stderr, "warning: size_t atomics are not lock-free on this "
                    "platform; this design won't work correctly.\n");
  }

  return total;
}

void block_buffer_queue_clear(block_buffer_queue_t *block_buffer_queue_clear) {}

block_buffer_queue_id_t
block_buffer_queue_assign_tid(block_buffer_global_t *block_buffer_global,
                              pthread_t tid, uint64_t key) {
  atomic_size_t idx = block_buffer_global_new(block_buffer_global, key);

  return key;
}

static buffer_queue_status_e
block_buffer_enqueue(block_buffer_queue_t *block_buffer_queue, block_t *block) {
  assert(block && block_buffer_queue);

  block_idx_t tail =
      atomic_load_explicit(&block_buffer_queue->tail, memory_order_relaxed);

  block_idx_t next_idx = (tail + 1) & BLOCK_QUEUE_IDX_MAX;

  if (next_idx ==
      atomic_load_explicit(&block_buffer_queue->head, memory_order_acquire)) {
    return QUEUE_STATUS_FULL;
  }

  block_buffer_queue->queue[tail] = *block;
  atomic_store_explicit(&block_buffer_queue->tail, next_idx,
                        memory_order_release);

  return QUEUE_STATUS_ENQUEUED;
}

static inline block_buffer_queue_t *
block_enqueue_find_buffer_queue(block_buffer_global_t *block_buffer_global,
                                block_t *block) {
  assert(block);

  size_t queue_idx = block->reg % BLOCK_QUEUES_TOTAL;

#ifdef DEBUG
  pid_t tid = pthread_self();
  validate_block_buffer_queue(queue_idx, tid);
#endif

  return &block_buffer_global->queues[queue_idx];
}

void block_add(block_t *block, import_t *importer) {
  assert(block && importer);

  importer->queue[importer->idx] = *block;
  importer->queue[importer->idx].used = true;
  importer->idx++;
}
static block_buffer_queue_t *
block_buffer_queue_get_with_qid(block_buffer_global_t *block_buffer_global,
                                block_buffer_queue_id_t queue_id) {
#ifdef DEBUG
  // TODO: validate that queue id is appropriate
#endif
  // get block_buffer_queue from general pool of known buffer queue
  return &block_buffer_global->queues[queue_id];
}

void block_enqueue(block_buffer_global_t *block_buffer_global, block_t *block) {
  assert(block && block_buffer_global);
  block_buffer_queue_t *buffer_queue =
      block_enqueue_find_buffer_queue(block_buffer_global, block);

  for (;;) {
    assert(buffer_queue);
    buffer_queue_status_e status = block_buffer_enqueue(buffer_queue, block);

    if (status == QUEUE_STATUS_FULL) {
      _Atomic size_t counter_temp =
          atomic_fetch_add_explicit(&counter, 1, memory_order_acquire);
      // printf("full! full count: %lu\n", counter_temp);
    } else {
      return;
    }
  }
}

static buffer_queue_status_e
block_buffer_dequeue(block_buffer_queue_t *block_buffer_queue,
                     block_t *block_to_use) {
  assert(block_buffer_queue);
  size_t head =
      atomic_load_explicit(&block_buffer_queue->head, memory_order_relaxed);

  if (head ==
      atomic_load_explicit(&block_buffer_queue->tail, memory_order_acquire)) {
    return QUEUE_STATUS_EMPTY;
  }

  *block_to_use = block_buffer_queue->queue[head];

  atomic_store_explicit(&block_buffer_queue->head,
                        (head + 1) & BLOCK_QUEUE_IDX_MAX, memory_order_release);

  return QUEUE_STATUS_DEQUEUED;
}

block_t block_dequeue(block_buffer_global_t *block_buffer_global,
                      block_buffer_queue_id_t queue_id) {
#ifdef DEBUG
  pid_t tid = pthread_self();
  validate_block_buffer_queue(queue_idx, tid);
#endif
  block_buffer_queue_t *buffer_queue =
      block_buffer_queue_get_with_qid(block_buffer_global, queue_id);

  block_t use_to_dequeue = {0};

  for (;;) {
    buffer_queue_status_e status =
        block_buffer_dequeue(buffer_queue, &use_to_dequeue);

    if (status == QUEUE_STATUS_EMPTY) {
      // handle bell wait;
    } else {
      assert(status == QUEUE_STATUS_DEQUEUED);
      // tell dock something has happened here
      return use_to_dequeue;
    }
  }

  return use_to_dequeue;
}

block_buffer_global_t *block_buffer_global_open(int shared_mem_fd) {
  block_buffer_global_t *open =
      mmap(NULL, sizeof(block_buffer_global_t), PROT_READ | PROT_WRITE,
           MAP_SHARED, shared_mem_fd, 0);

  assert(open != MAP_FAILED);
  return open;
}
