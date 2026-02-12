// #include <qcpy_error.h>
#include <qlog.h>

const char* replay_qlog_description[] = {
    [QLOG_NULL] = "qlog is null",          [QLOG_BAD_ENTRY] = "qlog bad entry",
    [QLOG_CORRUPT] = "qlog corruption",    [QLOG_OPTIMIZE_FAILED] = "qlog optimize failed",
    [QLOG_BAD_SIZING] = "qlog bad sizing", [QLOG_DELETE_FAILED] = "qlog failed to delete"};

qlog_t* qlog_init(uint8_t qubits)
{
    qlog_t* qlog;
    qlog = (qlog_t*)malloc(sizeof(qlog_t));
    if (!qlog)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG, qlog, QLOG_NULL);
    }
    memset(qlog, 0, sizeof(qlog_t));

    qlog->qubit_count = qubits;
    qlog->graph = qlog_graph_init(qlog->qubit_count);

    return qlog;
}

void qlog_delete(qlog_t* qlog)
{
    if (!qlog)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG, qlog, QLOG_NULL);
    }

    for (uint16_t i = qlog->entry_count; i > 0; --i)
    {
        qlog_entry_t* temp_ptr = qlog->last_entry;

        qlog->last_entry = qlog->last_entry->prev_entry;

        qlog_entry_delete(temp_ptr);

        if (temp_ptr)
        {
            // QCPY_ERROR(QCPY_ERROR_QLOG, qlog, QLOG_DELETE_FAILED);
        }
    }

    // qlog_stats_delete(qlog->stats);

    free(qlog);
    qlog = NULL;
    return;
}

void qlog_clear(qlog_t* qlog)
{
    if (!qlog)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG, qlog, QLOG_NULL);
    }

    qlog_entry_t* qlog_walker = qlog->entries;

    while (qlog_walker)
    {
        qlog_entry_t* temp_walker = qlog_walker;
        qlog_walker = qlog_walker->next_entry;
        qlog_entry_delete(temp_walker);

        if (temp_walker)
        {
        }
    }

    qlog->entry_count = 0;

    return;
}

void qlog_append(qlog_t* qlog, block_t block)
{
    assert(block.type == BLOCK_QLOG_ENTRY);
    if (!qlog)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG, qlog, QLOG_NULL);
    }

    if (block.qubits > qlog->qubit_count)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG, qlog, QLOG_BAD_SIZING);
    }
    if (!qlog->entries)
    {
        qlog->entries = qlog_entry_init(qlog->entry_count, block);
        qlog->last_entry = qlog->entries;
    }
    else
    {
        qlog->entries = qlog_entry_init(qlog->entry_count, block);
        qlog->last_entry->next_entry = qlog_entry_init(qlog->entry_count, block);

        if (!qlog->last_entry->prev_entry)
        {
            qlog->last_entry->next_entry->prev_entry = qlog->entries;
        }
        else
        {
            qlog->last_entry->next_entry->prev_entry = qlog->last_entry;
        }

        qlog->last_entry = qlog->last_entry->next_entry;
    }


    if (!qlog->last_entry)
    {
        // QCPY_ERROR(QCPY_ERROR_QLOG, qlog, QLOG_BAD_ENTRY);
    }
    ++(qlog->entry_count);

    qlog_graph_append(qlog->graph, qlog->last_entry);
}

void qlog_dump_content(qlog_t* qlog, bool verbose)
{
    if (!qlog)
    {
        return;
    }

    if (qlog->entry_count == 0)
    {
        printf("qlog is empty\n.");
        return;
    }

    if (verbose)
    {
    }

    qlog_entry_t* qlog_walker = qlog->entries;

    if (!qlog_walker)
    {
        return;
    }

    for (uint16_t i = 0; i < qlog->entry_count; ++i)
    {
        qlog_entry_dump_content(qlog_walker, verbose);
        qlog_walker = qlog_walker->next_entry;
    }

    return;
}

bool qlog_compare(qlog_t* qlog, qlog_t* to_compare)
{
    if (!qlog || !to_compare)
    {
        return false;
    }

    if (!qlog && !to_compare)
    {
        return true;
    }

    if (qlog->entry_count != to_compare->entry_count)
    {
        return false;
    }

    qlog_entry_t* qlog_walker = qlog->entries;
    qlog_entry_t* compare_walker = to_compare->entries;

    for (uint16_t i = 0; i < qlog->entry_count; ++i)
    {
        if (qlog_entry_compare(qlog_walker, compare_walker))
        {
            return false;
        }

        qlog_walker = qlog_walker->next_entry;
        compare_walker = compare_walker->next_entry;
    }

    return true;
}

qlog_t* qlog_combine(qlog_t* qlog, qlog_t* to_add)
{
    if (!qlog && !to_add)
    {
        return NULL;
    }

    if (!qlog)
    {
        return to_add;
    }

    if (!to_add || !to_add->entry_count)
    {
        return qlog;
    }

    qlog->entry_count += to_add->entry_count;
    qlog->last_entry->next_entry = to_add->entries;
    qlog->last_entry = to_add->last_entry;

    if (qlog->qubit_count < to_add->qubit_count)
    {
        qlog->qubit_count = to_add->qubit_count;
    }

    return qlog;
}
