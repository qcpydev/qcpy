#include <qlog_register.h>
#include <unistd.h>

qlog_register_buf_t* qlog_register_buf_init(qlog_register_buf_t* qlog_buffer)
{
    qlog_register_buf_t* new_qlog_buffer;
    new_qlog_buffer = (qlog_register_buf_t*)malloc(sizeof(qlog_register_buf_t));
    assert(new_qlog_buffer);
    memset(new_qlog_buffer, 0, sizeof(qlog_register_buf_t));

    if (!qlog_buffer)
    {
        return new_qlog_buffer;
    }

    if (!qlog_buffer->next)
    {
        qlog_buffer->next = new_qlog_buffer;
        qlog_buffer->last = new_qlog_buffer;
    }
    else
    {
        qlog_buffer->last->next = new_qlog_buffer;
        qlog_buffer->last = new_qlog_buffer;
    }


    return new_qlog_buffer;
}

void qlog_register_buf_delete(qlog_register_buf_t* qlog_register_buf)
{
    assert(qlog_register_buf);
    free(qlog_register_buf);
    qlog_register_buf = NULL;
}

void qlog_register_add(qlog_register_buf_t* qlog_register_buf, block_t block)
{
    assert(block.type == BLOCK_QLOG_ENTRY);

    uint64_t qubits = block.size;

    assert(qlog_register_buf);
    assert(qlog_register_buf->reg.id == block.reg);

    if (!qlog_register_buf->reg.qlog)
    {
        qlog_register_buf->reg.qlog = qlog_init(qubits);
    }

    assert(qlog_register_buf->reg.qlog);
    qlog_t* qlog = qlog_register_buf->reg.qlog;
    qlog_append(qlog, block);
}

qlog_register_buf_t* qlog_register_buf_find(qlog_register_buf_t* qlog_buffer, uint64_t id)
{
    qlog_register_buf_t* qlog_register_buf = qlog_buffer;
    assert(qlog_buffer);

    while (qlog_register_buf && qlog_register_buf->reg.id != id)
    {
        qlog_register_buf = qlog_register_buf->next;
    }

    if (!qlog_register_buf)
    {
        qlog_register_buf = qlog_register_buf_init(qlog_buffer);
        qlog_register_buf->reg.id = id;
    }

    return qlog_register_buf;
}
