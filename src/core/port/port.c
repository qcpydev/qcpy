#include <importer.h>
#include <port.h>
#include <qcpy_error.h>

bool port_closed = false;
import_t import_sorted;
pthread_t import_thread;
port_t* port = NULL;
sem_t* dock_sem;
sem_t* port_sem;

int shared_port_space = -1;

void port_import_init(int argc, char** argv)
{
    if (!argv || !argc)
    {
        assert(0);
    }

    int shared_port_space = shm_open(QCPY_PORT, O_RDWR, 0666);

    port = mmap(NULL, sizeof(port_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_port_space, 0);
    if (port == MAP_FAILED)
    {
        assert(0);
    }

    dock_sem = sem_open(DOCK_SEM, 0);
    if (dock_sem == SEM_FAILED)
    {
        perror("sem_open");
        assert(0);
    }

    port_sem = sem_open(PORT_SEM, 0);
    if (port_sem == SEM_FAILED)
    {
        perror("sem_open");
        assert(0);
    }


    // call import_sorted init
}
void* port_import(void* null)
{
    if (null)
    {
    }
    while (!port_closed)
    {
        sem_wait(port_sem);
        sem_wait(dock_sem);

        importer_sort_ported(port);
        port->size = 0;

        sem_post(dock_sem);
    }

    return NULL;
}

void* port_export()
{
    // TBD
    return NULL;
}

void port_close() { port_closed = true; }

void port_init(int argc, char** argv) { port_import_init(argc, argv); }
