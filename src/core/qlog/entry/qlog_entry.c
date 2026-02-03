#include <math.h>
// #include <qcpy_error.h>
#include <assert.h>
#include <qlog_entry.h>
#include <stdint.h>
#include <stdio.h>

#define BITPACK_MAX 0x3F
#define BITPACK_RANGE 6

qlog_entry_t* qlog_entry_init(uint64_t id, block_t block)
{
    qlog_entry_t* qlog_entry = (qlog_entry_t*)malloc(sizeof(qlog_entry_t));
    qlog_entry->entry_id = id;
    qlog_entry->qubit_bitmask = block.qubit_bitmask;
    qlog_entry->controlled_bitmask = block.controlled_bitmask;
    qlog_entry->target_bitmask = block.target_bitmask;
    qlog_entry->inverted = block.inverted;
    qlog_entry->gate_name = (base_gate_e)block.gate;
    qlog_entry->gate_type = (base_type_e)block.type;
    qlog_entry->qubit_count = block.qubits;
    qlog_entry->controlled_bitpack = block.controlled_bitpack;
    qlog_entry->target_bitpack = block.target_bitpack;
    qlog_entry->controlled_count = block.controlled_count;
    qlog_entry->target_count = block.target_count;

    // qlog_entry->gate_params = quantum_gate_params_init(gate, theta, phi, lambda);
    if (block.theta || block.phi || block.lmbda)
    {
    }

    return qlog_entry;
}

uint64_t qlog_entry_qubit_set_flags(uint8_t* qubits, uint8_t count)
{
    uint64_t qubit_int = 0;

    for (uint8_t i = 0; i < count; ++i)
    {
        qubit_int |= (1 << qubits[i]);
    }

    if (!qubit_int)
    {
    }

    return qubit_int;
}

uint8_t* qlog_entry_deconstruct_qubits(qlog_entry_t* qlog_entry)
{
    if (!qlog_entry)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, qlog_entry, QLOG_ENTRY_NULL);
    }


    uint8_t* qubit_decon = (uint8_t*)malloc(sizeof(uint8_t) * qlog_entry->qubit_count);

    if (!qubit_decon)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, qubit_decon, QLOG_ENTRY_NULL_QUBITS);
    }

    uint64_t qubit = qlog_entry->inverted ? 1ULL << 62 : 1;
    uint16_t i = 0;

    do
    {
        if (qubit & qlog_entry->qubit_bitmask)
        {
            // qubit_decon[i] = (uint8_t)log2(qubit);
            ++i;
        }

        if (qlog_entry->inverted)
        {
            qubit >>= 1;
        }
        else
        {
            qubit <<= 1;
        }

    } while (qubit > 0 && qubit <= 1ULL << 63);

    return qubit_decon;
}

void qlog_entry_delete(qlog_entry_t* qlog_entry)
{
    if (!qlog_entry)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, qlog_entry, QLOG_ENTRY_NULL);
    }

    // free(qlog_entry->gate_params);
    // free(qlog_entry->stats);
    free(qlog_entry);

    qlog_entry = NULL;

    return;
}

base_gate_e qlog_entry_qg_name(qlog_entry_t* qlog_entry) { return qlog_entry->gate_name; }

base_type_e qlog_entry_qg_type(qlog_entry_t* qlog_entry) { return qlog_entry->gate_type; }

void qlog_entry_dump_content(qlog_entry_t* qlog_entry, bool verbose)
{
    if (!qlog_entry)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, qlog_entry, QLOG_ENTRY_NULL);
    }

    printf("(");
    if (verbose)
    {
        printf("Qubit Count: %d", qlog_entry->qubit_count);
    }

    printf(" on: [");
    uint8_t* qubits = qlog_entry_deconstruct_qubits(qlog_entry);

    for (uint8_t i = 0; i < qlog_entry->qubit_count; ++i)
    {
        printf(" %d ", qubits[i]);
        if (i + 1 < qlog_entry->qubit_count)
        {
            printf(",");
        }
    }

    printf("])\n");
    return;
}

qlog_entry_t* qlog_entry_duplicates_to_clean(qlog_entry_t* qlog_entry)
{
    if (!qlog_entry || qlog_entry->prev_entry)
    {
        return NULL;
    }

    if (!qlog_entry_compare(qlog_entry, qlog_entry->prev_entry))
    {
        return qlog_entry;
    }

    uint64_t qlog_entry_count = 1;
    qlog_entry_t* prev = qlog_entry->prev_entry;

    while (prev->prev_entry && qlog_entry_compare(prev, prev->prev_entry))
    {
        ++qlog_entry_count;
        prev = prev->prev_entry;
    }

    if (qlog_entry_count & 1)
    {
        qlog_entry_t* next = qlog_entry->next_entry;

        if (!prev->next_entry || !prev->next_entry->next_entry)
        {
        }

        prev->next_entry->next_entry->next_entry = prev;
        prev->next_entry = prev->next_entry->next_entry;
        qlog_entry_delete(next);

        if (!next)
        {
            // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, next, QLOG_ENTRY_NULL);
        }
    }

    return prev;
}

const char* get_qlog_entry_gate(qlog_entry_t* qlog_entry)
{
    assert(qlog_entry);
    if (qlog_entry)
    {
    }

    return NULL;
}

const char* get_qlog_entry_gate_type(qlog_entry_t* qlog_entry)
{
    assert(qlog_entry);
    if (qlog_entry)
    {
    }

    return NULL;
}

bool qlog_entry_compare(qlog_entry_t* qlog_entry, qlog_entry_t* to_compare)
{
    if (!qlog_entry || !to_compare)
    {
        return false;
    }

    return qlog_entry->qubit_count == to_compare->qubit_count && qlog_entry->gate_type == to_compare->gate_type &&
           qlog_entry->gate_name == to_compare->gate_name && qlog_entry->qubit_bitmask == to_compare->qubit_bitmask;
}

uint16_t* qlog_entry_unpack(uint8_t qubits, uint64_t bitpack)
{
    uint16_t* unpacked = NULL;
    unpacked = (uint16_t*)malloc(2 * sizeof(qubits));

    for (uint16_t i = 0; i < qubits; ++i)
    {
        unpacked[i] = (bitpack >> (i * BITPACK_RANGE)) & BITPACK_MAX;
    }

    return unpacked;
}
