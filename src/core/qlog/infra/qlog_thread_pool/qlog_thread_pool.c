#include <block.h>
#include <importer.h>
#include <qcpy_error.h>
#include <qlog_thread_pool.h>

qlog_thread_pool_t qlog_thread_pool;
bool qlog_thread_pool_open = true;

void* qlog_thread_pool_worker(void* thread_index)
{
    uint64_t key = (uint64_t)thread_index;
    qlog_register_buf_t* qlog_buffer = qlog_register_buf_init(NULL);
    assert(qlog_buffer);

    qlog_register_buf_t* qlog_buffer_find = qlog_buffer;

    pthread_mutex_lock(&qlog_thread_pool.workers[key].lock);

    while (qlog_thread_pool_open)
    {
        pthread_cond_wait(&qlog_thread_pool.workers[key].cond, &qlog_thread_pool.workers[key].lock);
        qlog_thread_pool.workers[key].state = QLOG_PROCESS_START;

        import_block_t* block_queue = importer.queue[key];
        assert(block_queue);

        while (block_queue)
        {
            qlog_thread_pool.workers[key].state = QLOG_PROCESS_APPENDING;

            if (block_queue->block.reg != qlog_buffer_find->reg.id)
            {
                qlog_buffer_find = qlog_register_buf_find(qlog_buffer, block_queue->block.reg);
            }

            assert(qlog_buffer_find->reg.id == block_queue->block.reg);
            assert(qlog_buffer_find);

            qlog_register_add(qlog_buffer_find, block_queue->block);
            block_queue = block_queue->next;
        }

        qlog_thread_pool.workers[key].state = QLOG_PROCESS_DONE;

        importer_delete_queue(key);
        importer.count = 0;
        // possibly make copy of contents of block in thread stack and then delete queue
        // I like this idea more for the state machine we have

        qlog_thread_pool.workers[key].state = QLOG_PROCESS_EMPTY;
    }
    pthread_mutex_unlock(&qlog_thread_pool.workers[key].lock);

    return NULL;
}

void qlog_thread_pool_init()
{
    for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
    {
        pthread_mutex_init(&qlog_thread_pool.workers[i].lock, NULL);
        pthread_create(&qlog_thread_pool.workers[i].thread, NULL, qlog_thread_pool_worker, (void*)(intptr_t)i);
    }
}

void qlog_thread_pool_signal_worker(uint64_t key) { pthread_cond_signal(&qlog_thread_pool.workers[key].cond); }

void qlog_thread_pool_await()
{
    bool qlog_threads_done = false;
    // notify somehow that this is awaiting for thread pools to finish, easy non
    // lock holding method to wait before we allow more items to be added
    // I guess if we have not done anything in a second we should crap out huh?

    while (!qlog_threads_done)
    {
        for (uint64_t i = 0; i < IMPORTER_FUNNEL; ++i)
        {
            qlog_threads_done |= qlog_thread_pool.workers[i].state == QLOG_PROCESS_EMPTY;
        }
    }
}
