#define _GNU_SOURCE
#include <assert.h>
#include <base_tools.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

void *base_tools_create_shared_mem(size_t size, char *name) {
  int shared_mem_space = memfd_create(BLOCK_BUFFER_QUEUE_GLOBAL_NAME, 0);
  ;

  if (shared_mem_space == -1) {
    perror("shm_open");
    _exit(1);
  }

  if (ftruncate(shared_mem_space, size) < 0) {
    perror("ftruncate");
  }

  void *mem = mmap(NULL, size, PROT_ARGS, MAP_SHARED, shared_mem_space, 0);

  assert(mem != MAP_FAILED);

  memset(mem, 0, size);

  return mem;
}

int base_tools_create_shared_mem_new(size_t size, char *name, void **mem) {
  int shared_mem_space = memfd_create(BLOCK_BUFFER_QUEUE_GLOBAL_NAME, 0);
  ;

  if (shared_mem_space == -1) {
    perror("shm_open");
    _exit(1);
  }

  if (ftruncate(shared_mem_space, size) < 0) {
    perror("ftruncate");
    close(shared_mem_space);
  }

  void *mem_new = mmap(NULL, size, PROT_ARGS, MAP_SHARED, shared_mem_space, 0);

  if (mem_new == MAP_FAILED) {
    perror("mmap");
    close(shared_mem_space);
  }
  assert(mem_new != MAP_FAILED);

  memset(mem_new, 0, size);

  *mem = mem_new;
  return shared_mem_space;
}

sem_t *base_tools_create_shared_sem(char *name, unsigned int val) {
  sem_unlink(name);
  sem_t *sem = sem_open(name, OFLAG_SHARED_SEM_ARGS, MODE_SHARED_MEM, val);

  if (!sem) {
    perror("export_sem_failed");
  }

  return sem;
}

void *base_tools_open_shared_mem(size_t size, char *name) {
  int shared_mem = shm_open(name, O_RDWR, MODE_SHARED_MEM);

  if (shared_mem == -1) {
    perror("shm_open child");
    _exit(1);
  }

  void *mem = mmap(NULL, size, PROT_ARGS, MAP_SHARED, shared_mem, 0);

  assert(mem != MAP_FAILED);
  return mem;
}

sem_t *base_tools_open_shared_sem(char *name) {
  sem_t *shared_sem = sem_open(name, 0);

  if (shared_sem == SEM_FAILED) {
    perror("sem_open");
  }

  return shared_sem;
}
