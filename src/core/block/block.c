#include <block.h>
#include <qcpy_error.h>
#include <assert.h>

// this funciton should only run in debug mode
bool validate_block(block_t* block) {
  switch(block->type) {
    case (IMPORTER_QLOG_ENTRY):
      // check type of block and that it makes sense,
      break;
    default:
      assert(0);
  }
  return true;
}
