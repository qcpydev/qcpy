#include <qlog_register.h>
#include <stdlib.h>
#include <unistd.h>

qlog_register_buf_t *qlog_register_buf_init(qlog_register_buf_t **qlog_buffer,
                                            qubit_t qubits, reg_t id) {
  qlog_register_buf_t *new_qlog_buffer;
  new_qlog_buffer =
      (qlog_register_buf_t *)calloc(1, sizeof(qlog_register_buf_t));
  assert(new_qlog_buffer);

  new_qlog_buffer->qlog = qlog_init(qubits, id);
  assert(new_qlog_buffer->qlog);

  if (!*qlog_buffer) {
    *qlog_buffer = new_qlog_buffer;
  } else if (!(*qlog_buffer)->next) {
    (*qlog_buffer)->next = new_qlog_buffer;
    (*qlog_buffer)->last = new_qlog_buffer;
  } else {
    (*qlog_buffer)->last->next = new_qlog_buffer;
    (*qlog_buffer)->last = new_qlog_buffer;
  }

  return new_qlog_buffer;
}

void qlog_register_buf_delete(qlog_register_buf_t *qlog_register_buf) {
  assert(qlog_register_buf);
  free(qlog_register_buf);
  qlog_register_buf = NULL;
}

void qlog_register_add(qlog_register_buf_t *qlog_register_buf, block_t *block) {
  assert(block->type == BLOCK_QLOG_ENTRY);

  uint64_t qubits = block->size;

  assert(qlog_register_buf);
  assert(qlog_register_buf->qlog->id == block->reg);

  qlog_t *qlog = qlog_register_buf->qlog;

  assert(qlog_register_buf->qlog);
  assert(qlog_register_buf->qlog->id == block->reg);

  qlog_append(qlog, block);
}

qlog_register_buf_t *
qlog_register_buf_find(qlog_register_buf_t *thread_register_pool, reg_t id) {
  assert(thread_register_pool);

  while (thread_register_pool && thread_register_pool->qlog->id != id) {
    thread_register_pool = thread_register_pool->next;
  }

  return thread_register_pool;
}

void qlog_register_handle_input(qlog_register_buf_t **register_finder,
                                qlog_register_buf_t **thread_register_pool,
                                block_t *block) {

  if (block->type != BLOCK_QLOG_CREATE) {
    if ((*register_finder)->qlog->id != block->reg) {
      assert(register_finder && thread_register_pool);
      qlog_register_buf_find(*thread_register_pool, block->reg);
      assert((*register_finder)->qlog->id == block->reg);
    }
  }

  switch (block->type) {
  case BLOCK_FLUSH_ENTRY:
    qlog_flush_handler((*register_finder)->qlog, block->qubit_bitmask);
    break;
  case BLOCK_QLOG_ENTRY:
    qlog_register_add((*register_finder), block);
    break;
  case BLOCK_PRINT_ENTRY:
    qlog_print_handler((*register_finder)->qlog);
    break;
  case BLOCK_QLOG_CREATE:
    assert(sizeof(bitmask_t) == (sizeof(reg_t)));
    reg_t reg = block->qubit_bitmask;
    *register_finder =
        qlog_register_buf_init(thread_register_pool, block->qubits, reg);
    assert(register_finder);
    break;
  default:
    assert(0);
    break;
  }
}
