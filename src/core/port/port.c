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
    while (exporter_signal.count == 0) {
      pthread_cond_wait(&exporter_signal.cond, &exporter_signal.lock);
    }

    exporter_process();
  }

  pthread_mutex_unlock(&exporter_signal.lock);

  return not_used;
}

void port_close() { port_closed = true; }

void port_init(int argc, char **argv) {
  if (argc > 1 && argv) {
  }
  int block_buffer_memfd = atoi(argv[1]);
  int exporter_memfd = atoi(argv[2]);

  block_buffer_global_queues = block_buffer_global_open(block_buffer_memfd);
  assert(block_buffer_global_queues);

  exporter = exporter_init(exporter_memfd);
  assert(exporter);

  pthread_create(&export_thread, NULL, port_export, NULL);
}

void port_boot() {}
