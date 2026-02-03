#include <block.h>
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

#pragma once
#ifndef PORT_H
#define PORT_H

/*
 * Port for qcpy to handle requests from the user side.
 * This code creates a consumer-producer processes, this space is designed
 * to have a funnel for such ports and manage multiple processes
 *
 */

void port_init(int argc, char** argv);
void port_import_init(int argc, char** argv);
void port_close();
void* port_export();
void* port_import(void* null);

extern pthread_t import_thread;
#endif
