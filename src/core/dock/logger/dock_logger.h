#include <block.h>
#include <stdint.h>

#ifndef DOCK_LOGGER_H
#define DOCK_LOGGER_H

typedef struct dock_log_item_t dock_log_item_t;
struct dock_log_item_t {
  block_t block;
  dock_log_item_t *next;
};

typedef struct dock_log_t dock_log_t;
struct dock_log_t {
  dock_log_item_t *queue;
  dock_log_t *next;
  dock_log_item_t *last;
  dock_log_item_t *checkpoint;
  uint32_t reg;
};

typedef struct dock_logger_s {
  dock_log_t *logs;
  dock_log_t *last;
  dock_log_t *last_used;
} dock_logger_t;
;

dock_log_t *dock_log_init(uint32_t reg, block_t *block);
dock_log_item_t *dock_log_item_init(block_t *block);
void dock_log_append(uint32_t reg, block_t *block);

dock_log_t *dock_log_find(uint32_t reg);

int dock_log_fill_array(dock_log_t *dock_log, block_t *block);

void dock_log_append(uint32_t reg, block_t *block);

extern dock_logger_t dock_log;

#endif
