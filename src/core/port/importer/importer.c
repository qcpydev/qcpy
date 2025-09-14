#include <importer.h>
#include <qcpy_error.h>
#include <qlog_register.h>

import_t *import_queue = NULL;
import_sorted_t import_sorted;

void port_import_init() {
  if (import_queue) {
    return;
  }

  import_queue = mmap(NULL, sizeof(import_sorted_t), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  if (!import_queue) {
    assert(0);
  }

  sem_init(&import_queue->full, 1, 0);
  sem_init(&import_queue->empty, 1, 1);

  sem_init(&import_sorted.full, 1, 0);
  sem_init(&import_sorted.empty, 1, 1);
}

imported_item_t* port_imported_init(import_cxt_t *cxt) {
  if (!cxt) {
    assert(0);
  }

  import_cxt_t *copy_cxt = (import_cxt_t *)malloc(sizeof(import_cxt_t));

  if (!copy_cxt) {
    assert(0);
  }

  memcpy(cxt, copy_cxt, sizeof(import_cxt_t));

  imported_item_t *imported_item =
      (imported_item_t *)malloc(sizeof(imported_item_t));

  if (!imported_item) {
    assert(0);
  }

  imported_item->cxt = copy_cxt;

  return imported_item;
}

void port_import_sort(import_cxt_t *cxt) {
  if (!cxt) {
    assert(0);
  }

  if (import_sorted.count > IMPORT_MAX_SIZE) {
    assert(0);
  }

  imported_item_t *imported_item = port_imported_init(cxt);
  uint64_t reg = imported_item->cxt->item.qlog_register % IMPORT_MAX_SIZE;

  if (import_sorted.registers[reg]) {
    import_sorted.registers_last[reg]->next_cxt = imported_item;
    import_sorted.registers_last[reg] = imported_item;
  } else {
    import_sorted.registers[reg] = imported_item;
    import_sorted.registers_last[reg] = imported_item;

    ++(import_sorted.count);
  }
}

int port_import_handler() {
  uint64_t sorted_cnt = 0;

  sem_wait(&import_sorted.empty);

  for (uint16_t i = 0; i < import_queue->size; ++i) {
    port_import_sort(&(import_queue->queue[i]));
  }

  sorted_cnt = import_sorted.count;

  assert(import_sorted.count <= IMPORT_MAX_SIZE);

  sem_post(&import_sorted.full);

  return sorted_cnt;
}

void port_import_clear() {
  if (!import_queue) {
    assert(0);
  }

  memset(&import_queue->queue, 0, sizeof(import_cxt_t) * IMPORT_MAX_SIZE);

  import_queue->size = 0;
}
