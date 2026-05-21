#include <assert.h>
#include <dock_logger.h>
#include <stdlib.h>
#include <string.h>

dock_logger_t dock_log;

void dock_log_append(uint32_t reg, block_t *block) {
  assert(block);
  dock_log_t *to_append = dock_log.logs;

  if (!to_append) {
    to_append = dock_log_init(reg, block);
    dock_log.last_used = to_append;
    return;
  }

  to_append = dock_log_find(reg);

  if (!to_append) {
    to_append = dock_log_init(reg, block);
    dock_log.last_used = to_append;
    return;
  }

  assert(to_append && to_append->queue);

  dock_log_item_t *item = dock_log_item_init(block);

  if (!to_append->queue->next) {
    to_append->queue->next = item;
    to_append->last = item;
  } else {
    to_append->last->next = item;
    to_append->last = to_append->last->next;
  }
}

int dock_log_fill_array(dock_log_t *log, block_t *blocks) {
  assert(log && blocks);
  int count = 0;

  dock_log_item_t *queue = log->queue;
  assert(queue);

  if (log->checkpoint) {
    queue = log->checkpoint;
  }

  uint64_t i;

  for (i = 0; i < IMPORT_MAX_SIZE; ++i) {
    if (!queue) {
      return count;
    }

    memcpy(&blocks[i], &queue->block, sizeof(block_t));
    blocks[i].used = true;

    queue = queue->next;
    count++;
  }

  if (i != IMPORT_MAX_SIZE) {
    for (; i < IMPORT_MAX_SIZE; ++i) {
      memset(&blocks[i], 0, sizeof(block_t));
    }

    log->checkpoint = NULL;
    return IMPORT_MAX_SIZE - count;
  }

  if (queue) {
    log->checkpoint = queue->next;
  }

  return count;
}

int dock_get_qc_entries(uint32_t reg, block_t *blocks) {
  dock_log_t *dock_logger = dock_log_find(reg);
  return dock_log_fill_array(dock_logger, blocks);
}

dock_log_t *dock_log_init(uint32_t reg, block_t *block) {
  assert(block);

  dock_log_t *new_dock_log = (dock_log_t *)malloc(sizeof(dock_log_t));
  memset(new_dock_log, 0, sizeof(dock_log_t));

  new_dock_log->reg = reg;

  if (!dock_log.logs) {
    dock_log.logs = new_dock_log;
    dock_log.last = new_dock_log;
  } else {
    assert(dock_log.logs);
    dock_log.last->next = new_dock_log;
    dock_log.last = dock_log.last->next;
  }

  dock_log.last->queue = dock_log_item_init(block);
  assert(dock_log.last->queue);
  return new_dock_log;
}

dock_log_item_t *dock_log_item_init(block_t *block) {
  assert(block);

  dock_log_item_t *dock_log_item = NULL;

  dock_log_item = (dock_log_item_t *)malloc(sizeof(dock_log_item_t));
  memset(dock_log_item, 0, sizeof(dock_log_item_t));
  memcpy(&dock_log_item->block, block, sizeof(block_t));

  return dock_log_item;
}

dock_log_t *dock_log_find(uint32_t reg) {
  if (dock_log.last_used && dock_log.last_used->reg == reg) {
    return dock_log.last_used;
  }

  dock_log_t *dock_log_walker = dock_log.logs->next;

  while (dock_log_walker && dock_log_walker->reg != reg) {
    dock_log_walker = dock_log_walker->next;
  }

  if (dock_log_walker) {
    dock_log.last_used = dock_log_walker;
  }

  return dock_log_walker;
}
