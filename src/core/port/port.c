#include <importer.h>
#include <port.h>
#include <qcpy_error.h>
#include <qlog_infra.h>

bool port_closed = false;
ports_t ports = {0};

void *port_import() {
  while (!port_closed) {
    sem_wait(&import_queue->full);

    if (import_queue->size) {
      pthread_mutex_lock(&ports.processing);
      port_import_handler();
      pthread_mutex_unlock(&ports.processing);
    }

    port_import_clear();
    assert(!import_queue->size);

    sem_post(&import_queue->empty);
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

  pthread_create(&ports.import_thread, NULL, port_import, NULL);
  pthread_create(&ports.export_thread, NULL, port_export, NULL);
}
