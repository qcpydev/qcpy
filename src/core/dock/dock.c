#include <assert.h>
#include <dock.h>
#include <fcntl.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

import_t* importer;
export_t* exporter;
sem_t* dock_import_sem;
sem_t* port_import_sem;

sem_t* dock_export_sem;
sem_t* port_export_sem;

int shared_import_space;
int shared_export_space;

static void dock_create_importer()
{
    assert(!importer);
    shared_import_space = shm_open(QCPY_IMPORT, OFLAG_SHARED_MEM_ARGS, MODE_SHARED_MEM);
    if (shared_import_space < 0)
    {
        perror("shm_open");
        _exit(1);
        assert(0);
    }

    if (ftruncate(shared_import_space, sizeof(import_t)) < 0)
    {
        perror("ftruncate");
        assert(0);
    }

    importer = mmap(NULL, sizeof(*importer), PROT_ARGS, MAP_SHARED, shared_import_space, 0);

    if (importer == MAP_FAILED)
    {
        perror("map_failed");
        assert(0);
    }

    memset(importer, 0, sizeof(import_t));
    sem_unlink(DOCK_IMPORT_SEM);
    dock_import_sem = sem_open(DOCK_IMPORT_SEM, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, IMPORT_MAX_SIZE);

    sem_unlink(PORT_IMPORT_SEM);
    port_import_sem = sem_open(PORT_IMPORT_SEM, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 0);

    if (!dock_import_sem)
    {
        perror("import_sem_failed");
        assert(0);
    }

    if (!port_import_sem)
    {
        perror("import_sem_failed");
        assert(0);
    }
}

static void dock_create_exporter()
{
    assert(!exporter);
    shared_export_space = shm_open(QCPY_EXPORT, OFLAG_SHARED_MEM_ARGS, MODE_SHARED_MEM);
    if (shared_export_space < 0)
    {
        perror("shm_open");
        _exit(1);
        assert(0);
    }

    if (ftruncate(shared_export_space, sizeof(export_t)) < 0)
    {
        perror("ftruncate");
        assert(0);
    }

    exporter = mmap(NULL, sizeof(*exporter), PROT_ARGS, MAP_SHARED, shared_export_space, 0);

    if (exporter == MAP_FAILED)
    {
        perror("map_failed");
        assert(0);
    }

    memset(exporter, 0, sizeof(export_t));
    sem_unlink(DOCK_EXPORT_SEM);
    dock_export_sem = sem_open(DOCK_EXPORT_SEM, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, IMPORT_MAX_SIZE);

    sem_unlink(PORT_EXPORT_SEM);
    port_export_sem = sem_open(PORT_EXPORT_SEM, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, 0);

    if (!dock_export_sem)
    {
        perror("export_sem_failed");
        assert(0);
    }

    if (!port_export_sem)
    {
        perror("export_sem_failed");
        assert(0);
    }
}

void dock_port_init()
{
    dock_create_exporter();
    dock_create_importer();
}

char* dock_init(char* args[])
{
    if (!args)
    {
        return NULL;
    }

    char* exec_name = args[0];
    return exec_name;
}

void* dock_run_boot(void* empty)
{
    if (empty)
    {
    }
    assert(!empty);
    assert(boot_thread_args);

    dock_port_init();

    pid_t pid = fork();
    if (pid == 0)
    {
        prctl(PR_SET_PDEATHSIG, SIGTERM);
        char* args_test[] = {NULL};
        execvp(boot_thread_args->exec_name, args_test);
        perror("execvp failed");
        _exit(1);
    }

    return NULL;
}

static void dock_ship_to_port()
{
    for (uint64_t i = 0; i < IMPORT_MAX_SIZE; ++i)
    {
        sem_post(port_import_sem);
    }
}

int dock_add(block_t* block)
{
    sem_wait(dock_import_sem);
    block_add(block, importer);

    if (importer->dock_idx % IMPORT_MAX_SIZE == 0 && importer->dock_idx != 0)
    {
        dock_ship_to_port();
    }

    return 1;
}

void dock_get_qc_state(int flush_reg, bool is_print)
{

    sem_wait(dock_import_sem);
    assert(port);
    importer->flushing = true;
    importer->flush_reg = flush_reg;
    dock_ship_to_port();
    if (is_print)
    {
    }

    return;
}
