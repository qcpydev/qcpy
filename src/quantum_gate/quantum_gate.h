#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../global_types/global.h"

typedef struct quantum_gate_params_def {
  float16 theta;
  float16 phi;
  float16 lambda;
  bool has_theta;
  bool has_phi;
  bool has_lambda;
} quantum_gate_params_def;

typedef struct quantum_gate_def {
  global_gate_name quantum_gate_name;
  global_gate_type quantum_gate_type;
  struct global_matrix_def** quantum_gate_matrix;
  struct quantum_gate_params_def* quantum_gate_params;
  uint16_t quantum_gate_size;
} quantum_gate_def;

quantum_gate_params_def* quantum_gate_params_zeroed();

quantum_gate_def* quantum_gate_init(uint16_t size, global_gate_name gate_name, global_gate_type gate_type, global_matrix_def **matrix, struct quantum_gate_params_def *quantum_gate_params);
void quantum_gate_delete(struct quantum_gate_def *quantum_gate);
global_matrix_def* quantum_gate_export();
void quantum_gate_import();
bool quantum_gate_compare_params(struct quantum_gate_def *qg, struct quantum_gate_def *qg_to_compare);

struct quantum_gate_def* quantum_gate_get_gate(int qg_name, float theta, float phi, float lambda);
struct quantum_gate_def* quantum_gate_identity(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_hadamard(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_paulix(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_pauliz(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_pauliy(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_p(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_s(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_sdg(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_t(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_tdg(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_rz(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_ry(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_rx(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_sx(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_sxdg(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_u(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_cx(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_ch(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_cy(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_cz(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_crx(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_cry(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_crz(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_cr1(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_csx(struct quantum_gate_params_def* qg_params);
struct quantum_gate_def* quantum_gate_csxdg(struct quantum_gate_params_def* qg_params);

struct quantum_gate_def* quantum_gate_braket_zero();
struct quantum_gate_def* quantum_gate_braket_one();

struct quantum_gate_def** quantum_gate_get_arr_identity_gates(uint16_t size);
