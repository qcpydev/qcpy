#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "port_stats.h"
#include "../global_types/global.h"

typedef struct state_split_def {
  float16* state_split_real;
  float16* state_split_imag;
  uint32_t state_split_size;
} state_split_def;

typedef struct matrix_split_def {
  float16** matrix_split_real;
  float16** matrix_split_imag;
  uint32_t matrix_split_size;
} matrix_split_def;


struct matrix_split_def* matrix_split_init(struct global_matrix_def* matrix);
void matrix_split_delete(struct matrix_split_def* matrix_split);
float16** matrix_split_get_imag(struct matrix_split_def* matrix_split);
float16** matrix_split_get_real(struct matrix_split_def* matrix_split);

struct state_split_def* state_split_init(struct global_state_def* state);
void state_split_delete(struct state_split_def* state_split);
float16* state_split_get_imag(struct global_state_def* state);
float16* state_split_get_real(struct global_state_def* state);

