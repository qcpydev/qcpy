#include <importer.h>
#include <port.h>
#include <qcpy_error.h>

bool port_closed = false;
ports_t ports = {0};

int port_create_cxt_request(block_t *ctx) {
  assert(ctx);
  pthread_mutex_lock(&ports.processing);

  switch (ctx->type) {
  case (IMPORTER_QLOG_ENTRY):
    port_import_enqueue(ctx);
    break;
  default:
    assert(0);
  }

  pthread_mutex_unlock(&ports.processing);
  return 0;
}

void *port_import() {
  while (!port_closed) {
    pthread_mutex_lock(&ports.processing);

    if (import_queue->size == IMPORT_MAX_SIZE) {
      port_import_handler();
    }

    pthread_mutex_unlock(&ports.processing);
  }

  return NULL;
}

void *port_export() {
  // TBD
  return NULL;
}

void port_close() {
  pthread_mutex_lock(&ports.processing);
  port_closed = true;
  pthread_mutex_lock(&ports.processing);

  pthread_join(ports.export_thread, NULL);
  pthread_join(ports.import_thread, NULL);
}

void port_init() {
  port_import_init();
  // port_export_init();

  pthread_mutex_init(&ports.processing, NULL);

  // pthread_create(&ports.export_thread, NULL, port_export, NULL);
}

int port_import_queue_size() {
  uint64_t size = 0;
  pthread_mutex_lock(&ports.processing);
  pthread_mutex_lock(&import_sorted.sorting);
  size = import_queue->size;
  pthread_mutex_unlock(&import_sorted.sorting);
  pthread_mutex_unlock(&ports.processing);
  return size;
}
