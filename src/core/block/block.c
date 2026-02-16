#include <assert.h>
#include <block.h>
#include <qcpy_error.h>

// this funciton should only run in debug mode
bool validate_block(block_t *block) {
  switch (block->type) {
  case (BLOCK_QLOG_ENTRY):
    // check type of block and that it makes sense,
    break;
  default:
    assert(0);
  }
  return true;
}

void block_add(block_t *block, import_t *importer) {
  assert(block && importer);
  int idx = importer->dock_idx % IMPORT_MAX_SIZE;
  importer->queue[idx] = *block;
  importer->queue[idx].used = true;
  importer->dock_idx++;
}
