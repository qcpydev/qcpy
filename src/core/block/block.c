#include <assert.h>
#include <block.h>
#include <qcpy_error.h>

// this funciton should only run in debug mode
bool validate_block(block_t* block)
{
    switch (block->type)
    {
    case (BLOCK_QLOG_ENTRY):
        // check type of block and that it makes sense,
        break;
    default:
        assert(0);
    }
    return true;
}

void port_add(block_t* block, port_t* port)
{
    assert(block && port);
    // printf("content of block: %lu, %lu, %lu, %lu\n", block->reg, block->gate_type, block->qubits,
    // block->qubit_bitmask);
    port->queue[port->size] = *block;
    ++port->size;
}
