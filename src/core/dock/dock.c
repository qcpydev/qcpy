#include <dock.h>
#include <unistd.h>
#include <qcpy_error.h>
#include <assert.h>

void port_import_init() {
  assert(!import_queue);

  import_queue = mmap(NULL, sizeof(import_sorted_t), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (!import_queue) {
    assert(0);
  }

  memset(import_queue, 0, sizeof(import_sorted_t));
  memset(&import_queue->queue, 0, sizeof(block_t) * IMPORT_MAX_SIZE);

  sem_init(&import_queue->full, 1, 0);
  sem_init(&import_queue->empty, 1, 1);

  pthread_mutex_init(&import_sorted.sorting, NULL);

  for (uint64_t i = 0; i < IMPORT_MAX_SIZE; ++i) {
    pthread_mutex_init(&import_sorted.registers_lock[i], NULL);
  }

}
char* dock_init(char* args[]) {
  if (!args) {
    return NULL;
  }

  char* exec_name = args[0];
  ++args;
  return exec_name;
}

void dock_run_boot(char* boot_exec, char* args[]) {
  pid_t pid = fork();

  if (pid == 0) {
    execvp(boot_exec, args);
  }
}

