#include <block.h>
#include <exporter.h>
#include <importer.h>
#include <port.h>
#include <qcpy_error.h>
#include <stdio.h>
#include <stdlib.h>

import_t* importer = NULL;
export_t* exporter = NULL;

sem_t* dock_import_sem;
sem_t* port_import_sem;

sem_t* dock_export_sem;
sem_t* port_export_sem;

int shared_import_space;
int shared_export_space;

pthread_t import_thread;
pthread_t export_thread;
bool port_closed = false;

void port_import_init()
{
    shared_import_space = shm_open(QCPY_IMPORT, O_RDWR, MODE_SHARED_MEM);

    if (shared_import_space == -1)
    {
        perror("shm_open child");
        exit(1);
    }

    importer = mmap(NULL, sizeof(*importer), PROT_ARGS, MAP_SHARED, shared_import_space, 0);
    if (importer == MAP_FAILED)
    {
        assert(0);
    }


    dock_import_sem = sem_open(DOCK_IMPORT_SEM, 0);
    if (dock_import_sem == SEM_FAILED)
    {
        perror("sem_open");
        assert(0);
    }

    port_import_sem = sem_open(PORT_IMPORT_SEM, 0);

    if (port_import_sem == SEM_FAILED)
    {
        perror("sem_open");
        assert(0);
    }

    importer->ready = true;
}

void port_export_init()
{
    shared_export_space = shm_open(QCPY_EXPORT, O_RDWR, MODE_SHARED_MEM);

    if (shared_export_space == -1)
    {
        perror("shm_open child");
        exit(1);
    }

    exporter = mmap(NULL, sizeof(*exporter), PROT_ARGS, MAP_SHARED, shared_export_space, 0);
    if (exporter == MAP_FAILED)
    {
        assert(0);
    }


    dock_export_sem = sem_open(DOCK_EXPORT_SEM, 0);
    if (dock_export_sem == SEM_FAILED)
    {
        perror("sem_open");
        assert(0);
    }

    port_export_sem = sem_open(PORT_EXPORT_SEM, 0);

    if (port_export_sem == SEM_FAILED)
    {
        perror("sem_open");
        assert(0);
    }

    exporter->ready = true;
}

void* port_import(void* null)
{
    while (!port_closed)
    {
        importer_sort_ported(importer);
    }

    return null;
}

void* port_export(void* null)
{
    while (!port_closed)
    {
        exporter_quack(exporter);
    }
    // TBD
    return null;
}

void port_close() { port_closed = true; }

void port_init(int argc, char** argv)
{
    if (argc > 1 && argv)
    {
    }
    port_import_init();
    port_export_init();
}
