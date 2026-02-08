#include <block.h>
#include <importer.h>
#include <port.h>
#include <qcpy_error.h>
#include <qlog_infra.h>

import_sort_t importer_sort;


void importer_sort_init()
{
    pthread_mutex_init(&importer_sort.lock, NULL);

    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
    {
        pthread_mutex_init(&importer_sort.queue_lock[i], NULL);
    }

    memset(importer_sort.queue, 0, sizeof(import_block_t*) * IMPORTER_FUNNEL);
    memset(importer_sort.queue_last, 0, sizeof(import_block_t*) * IMPORTER_FUNNEL);
}

void importer_sort_append(block_t block)
{
    import_block_t* import_block = import_block_init();
    assert(import_block);
    uint64_t index = block.reg % IMPORTER_FUNNEL;

    if (!importer_sort.queue[index])
    {
        importer_sort.queue[index] = import_block;
        importer_sort.queue_last[index] = import_block;
    }
    else
    {
        importer_sort.queue_last[index]->next = import_block;
        importer_sort.queue_last[index] = importer_sort.queue_last[index]->next;
    }


    ++importer_sort.count;
}

void importer_sort_ported(import_t* importer)
{
    assert(port);
    for (uint64_t i = 0; i < IMPORT_MAX_SIZE; ++i)
    {
        sem_wait(port_import_sem);
        int idx = importer->port_idx % IMPORT_MAX_SIZE;
        if (importer->queue[idx].used)
        {
            importer_sort_append(importer->queue[idx]);
            importer->queue[idx].used = false;
        }
        importer->port_idx++;
        sem_post(dock_import_sem);
    }

    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
    {
        if (importer_sort.queue[i])
        {
            if (importer->flushing && importer->flush_reg % IMPORTER_FUNNEL != i)
            {
                continue;
            }
            pthread_cond_signal(&qlog_thread_pool.workers[i].cond);
        }
    }
    qlog_thread_pool_await();

    importer->flushing = false;
    importer->flush_reg = 0;
}

void importer_delete_queue(uint64_t idx)
{
    assert(idx < importer_sort_FUNNEL);

    import_block_t* import_block_queue = importer_sort.queue[idx];
    importer_sort.queue[idx] = NULL;
    importer_sort.queue_last[idx] = NULL;

    while (import_block_queue)
    {
        import_block_t* delete_block = import_block_queue;
        import_block_queue = import_block_queue->next;
        import_block_delete(delete_block);
    }
}

void importer_sort_clear()
{
    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
    {
        importer_delete_queue(i);
    }

    importer_sort.count = 0;
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
