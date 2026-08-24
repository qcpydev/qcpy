#include <block.h>
#include <exporter.h>
#include <importer.h>
#include <port.h>
#include <qcpy_error.h>
#include <stdio.h>
#include <stdlib.h>

export_t *exporter = NULL;

sem_t *dock_export_sem;
sem_t *port_export_sem;
pthread_t export_thread;

pthread_t import_thread;

bool port_closed = false;

void *port_import(void *not_used) {
  (void)not_used;
  while (!port_closed) {
    importer_sort_ported(importer);
  }

  return NULL;
}

void *port_export(void *not_used) {

  pthread_mutex_lock(&exporter_signal.lock);

  while (!port_closed) {
    pthread_cond_wait(&exporter_signal.cond, &exporter_signal.lock);
    printf("exporter got the trigger\n");
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

  importer_open();
  exporter_init();
}

void port_boot() {

  pthread_create(&import_thread, NULL, port_import, NULL);

  pthread_create(&export_thread, NULL, port_export, NULL);

  pthread_join(import_thread, NULL);
  pthread_join(export_thread, NULL);
}
