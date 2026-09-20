#include <block.h>
#include <exporter.h>
#include <port.h>
#include <qcpy_error.h>
#include <stdlib.h>

export_t *exporter = NULL;
pthread_t export_thread;

bool port_closed = false;

block_buffer_global_t *block_buffer_global_queues = NULL;

void *port_export(void *not_used) {
  pthread_mutex_lock(&exporter_signal.lock);
  while (!port_closed) {
    pthread_cond_wait(&exporter_signal.cond, &exporter_signal.lock);
    // while (exporter_signal.items) {
    //  exporter_process();
    //}
  }

  pthread_mutex_unlock(&exporter_signal.lock);

  return not_used;
}

void port_close() { port_closed = true; }

void port_init(int argc, char **argv) {
  if (argc > 1 && argv) {
  }
  int shared_mem_fd = atoi(argv[1]);

  block_buffer_global_queues = block_buffer_global_open(shared_mem_fd);
  assert(block_buffer_global_queues);

  exporter_init();
}

void port_boot() {}
