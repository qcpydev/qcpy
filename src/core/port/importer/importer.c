#include <importer.h>
#include <port.h>
#include <qcpy_error.h>
#include <qlog_infra.h>

import_t importer;

void importer_init()
{
    pthread_mutex_init(&importer.lock, NULL);

    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
    {
        pthread_mutex_init(&importer.queue_lock[i], NULL);
    }

    memset(importer.queue, 0, sizeof(import_block_t*) * IMPORTER_FUNNEL);
    memset(importer.queue_last, 0, sizeof(import_block_t*) * IMPORTER_FUNNEL);
}

void importer_append(block_t block)
{
    import_block_t* import_block = import_block_init();
    assert(import_block);
    uint64_t index = block.reg % IMPORTER_FUNNEL;

    if (!importer.queue[index])
    {
        importer.queue[index] = import_block;
        importer.queue_last[index] = import_block;
    }
    else
    {
        importer.queue_last[index]->next = import_block;
        importer.queue_last[index] = importer.queue_last[index]->next;
    }


    ++importer.count;
}

void importer_sort_ported(port_t* port)
{
    assert(port);

    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
    {
        pthread_mutex_lock(&importer.queue_lock[i]);
        importer_append(port->queue[i]);
    }

    // lock enqueued indices, notify qlog_infra to handle whatever they need to
    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
    {
        pthread_mutex_unlock(&importer.queue_lock[i]);
        if (importer.queue[i])
        {
            pthread_cond_signal(&qlog_thread_pool.workers[i].cond);
        }
    }

    qlog_thread_pool_await();
}

void importer_delete_queue(uint64_t idx)
{
    assert(idx < IMPORTER_FUNNEL);

    import_block_t* import_block_queue = importer.queue[idx];
    importer.queue[idx] = NULL;
    importer.queue_last[idx] = NULL;

    while (import_block_queue)
    {
        import_block_t* delete_block = import_block_queue;
        import_block_queue = import_block_queue->next;
        import_block_delete(delete_block);
    }
}

void importer_clear()
{
    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
    {
        importer_delete_queue(i);
    }

    importer.count = 0;
}

import_block_t* import_block_init()
{
    import_block_t* import_block;
    import_block = (import_block_t*)malloc(sizeof(import_block_t));
    memset(import_block, 0, sizeof(import_block_t));
    assert(import_block);

    return import_block;
}

void import_block_delete(import_block_t* import_block)
{
    assert(import_block);
    memset(import_block, 0, sizeof(import_block_t));
    free(import_block);
    return;
}
