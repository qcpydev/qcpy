#include <assert.h>
#include <dock.h>
#include <fcntl.h>
#include <pthread.h>
#include <qcpy_error.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <unistd.h>

port_t* port = NULL;
sem_t* dock_sem;
sem_t* port_sem;
int shared_port_space = -1;

void dock_port_init()
{
    assert(!port);

    shared_port_space = shm_open(QCPY_PORT, O_CREAT | O_RDWR, 0666);
    if (shared_port_space < 0)
    {
        assert(0);
    }

    if (ftruncate(shared_port_space, sizeof(port_t)) < 0)
    {
        assert(0);
    }

    port = mmap(NULL, sizeof(port_t), PROT_READ | PROT_WRITE, MAP_SHARED, shared_port_space, 0);

    memset(port, 0, sizeof(port_t));
    if (port == MAP_FAILED)
    {
        assert(0);
    }

    sem_unlink(DOCK_SEM);
    dock_sem = sem_open(DOCK_SEM, O_CREAT, 0666, 1);

    sem_unlink(PORT_SEM);
    port_sem = sem_open(PORT_SEM, O_CREAT, 0666, 0);
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
        // will parse args eventually and pass it to qcpy_core
        prctl(PR_SET_PDEATHSIG, SIGTERM);
        char* args_test[] = {NULL};
        execvp(boot_thread_args->exec_name, args_test);
    }

    return NULL;
}

int dock_add(block_t* block)
{
    sem_wait(dock_sem);
    port_add(block, port);
    if (port->size == IMPORT_MAX_SIZE)
    {
        sem_post(port_sem);
    }
    sem_post(dock_sem);
    return 1;
}
