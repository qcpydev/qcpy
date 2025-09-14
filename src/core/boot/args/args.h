#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef ARGS
#define ARGS

typedef enum {
  DEFAULT_BOOT = 0x1,
  DEBUG_BOOT = 0x2,
  DISABLE_STATS = 0x4,
  USE_GPU = 0x8,
  SPARSE_MATRIX = 0x16,
} base_arg_value_e;

typedef struct args_global_s {
  uint64_t base_arg_values;

} args_global_t;

extern const char *arg_names;
extern const args_global_t qcpy_args;

bool set_base_arg(const char *arg_name);
bool get_base_arg();

#endif
