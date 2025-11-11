#include <exporter.h>
#include <fcntl.h>
#include <importer.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <block.h>

#pragma once
#ifndef PORT_H
#define PORT_H

/*
 * Port for qcpy to handle requests from the user side.
 * This code creates a consumer-producer processes, this space is designed
 * to have a funnel for such ports and manage multiple processes
 *
 */

typedef struct ports_s {
  pthread_t import_thread;
  pthread_t export_thread;
  pthread_mutex_t processing;
} ports_t;

extern ports_t ports;

void port_init();
void port_close();
void *port_export();
void *port_import();
int port_create_cxt_request(block_t *ctx);
int port_import_queue_size();

#endif
