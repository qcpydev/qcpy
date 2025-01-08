#include "quantum_gate.h"

#define QG_HAS_NO_PARAMS(qg_name) (!(QG_HAS_THETA_PARAMS(qg_name) || QG_HAS_PHI_PARAMS(qg_name) || QG_HAS_LAMBDA_PARAMS(qg_name)))
#define QG_HAS_THETA_PARAMS(qg_name) (qg_name == GLOBAL_GATE_PHASE || qg_name == GLOBAL_GATE_RZ || qg_name == GLOBAL_GATE_RX || qg_name == GLOBAL_GATE_RY || qg_name == GLOBAL_GATE_U)
#define QG_HAS_LAMBDA_PARAMS(qg_name) (qg_name == GLOBAL_GATE_U)
#define QG_HAS_PHI_PARAMS(qg_name) (qg_name == GLOBAL_GATE_U)
#define QG_SINGLE_GATE_SIZE (2)
#define QG_HAS_ANY_SET_PARAMS(qg_params) (qg_params->has_theta || qg_params->has_phi || qg_params->has_lambda)
#define QG_HAS_ALL_SET_PARAMS(qg_params) (qg_params->has_theta && qg_params->has_phi && qg_params->has_lambda)
#define QG_HAS_THETA_SET_PARAMS(qg_params) (qg_params->has_theta && !(qg_params->has_phi && qg_params->has_lambda))

typedef struct quantum_gate_def* (*qg_func)(struct quantum_gate_params_def* qg_params);
const qg_func qg_init_functions[] = {
  [GLOBAL_GATE_IDENTITY] = quantum_gate_identity,
  [GLOBAL_GATE_HADAMARD] = quantum_gate_hadamard,
  [GLOBAL_GATE_PAULIX] = quantum_gate_paulix,
  [GLOBAL_GATE_PAULIZ] = quantum_gate_pauliy,
  [GLOBAL_GATE_PAULIY] = quantum_gate_pauliz,
  [GLOBAL_GATE_PHASE] = quantum_gate_p,
  [GLOBAL_GATE_S] = quantum_gate_s,
  [GLOBAL_GATE_TDG] = quantum_gate_tdg,
  [GLOBAL_GATE_RZ] = quantum_gate_rz,
  [GLOBAL_GATE_RY] = quantum_gate_ry,
  [GLOBAL_GATE_RX] = quantum_gate_rx,
  [GLOBAL_GATE_T] = quantum_gate_t,
  [GLOBAL_GATE_SDG] = quantum_gate_sdg,
  [GLOBAL_GATE_SX] = quantum_gate_sx,
  [GLOBAL_GATE_SXDG] = quantum_gate_sxdg,
  [GLOBAL_GATE_U] = quantum_gate_u,
}; 

struct quantum_gate_def* quantum_gate_get_gate(int qg_name, float theta, float phi, float lambda) {
  if (!QG_HAS_PHI_PARAMS(qg_name) || !QG_HAS_NO_PARAMS(qg_name) || !QG_HAS_THETA_PARAMS(qg_name) || !QG_HAS_LAMBDA_PARAMS(qg_name)) {
    return NULL;
  }
  struct quantum_gate_params_def* qg_params = malloc(sizeof(quantum_gate_params_def));
  if (QG_HAS_THETA_PARAMS(qg_name)) {
    qg_params->theta = theta;
    qg_params->has_theta = true;
  }
  if (QG_HAS_PHI_PARAMS(qg_name)) {
    qg_params->phi = phi;
    qg_params->has_phi = true;
  }
  if (QG_HAS_LAMBDA_PARAMS(qg_name)) {
    qg_params->lambda = lambda;
    qg_params->lambda = true;
  }
  struct quantum_gate_def* gate = qg_init_functions[qg_name](qg_params);
  if (gate == NULL) {
    printf("INTERNAL ERROR: on gate %d \n", qg_name);
    exit(1);
  }
  return gate;
}

quantum_gate_def* quantum_gate_init(uint16_t size, global_gate_name gate_name, global_gate_type gate_type, global_matrix_def **matrix, struct quantum_gate_params_def *qg_params) {
  if (!matrix || !matrix[0] || !qg_params) {
    return NULL;
  }
  struct quantum_gate_def* quantum_gate = (quantum_gate_def*)malloc(sizeof(quantum_gate_def));
  quantum_gate->quantum_gate_size = size;
  quantum_gate->quantum_gate_name = gate_name;
  quantum_gate->quantum_gate_type = gate_type;
  quantum_gate->quantum_gate_matrix = matrix;
  quantum_gate->quantum_gate_params = qg_params;
  return quantum_gate;
}

void quantum_gate_delete(struct quantum_gate_def* quantum_gate) {
  if (!quantum_gate) {
    return;
  }
  global_matrix_delete(quantum_gate->quantum_gate_matrix, quantum_gate->quantum_gate_size);
  free(quantum_gate->quantum_gate_params);
  quantum_gate->quantum_gate_params = NULL;
  free(quantum_gate);
  quantum_gate = NULL;
  return;
}

struct quantum_gate_def* quantum_gate_identity(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  if (!matrix) {
    return NULL;
  }
  matrix[0][0].real = 1;
  matrix[1][1].real = 1;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_IDENTITY, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_hadamard(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_alloc(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 1 / sqrt(2);
  matrix[0][0].imag = 0;
  matrix[0][1].real = 1 / sqrt(2);
  matrix[0][1].imag = 0;
  matrix[1][0].real = 1 / sqrt(2);
  matrix[1][0].imag = 0;
  matrix[1][1].real = -(1 / sqrt(2));
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_HADAMARD, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_paulix(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][1].real = 1;
  matrix[1][0].real = 1;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_PAULIX, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_pauliz(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 1;
  matrix[1][1].real = -1;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_PAULIZ, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_pauliy(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][1].imag = -1;
  matrix[1][0].imag = 1;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_PAULIZ, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_p(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 1;
  matrix[1][1].imag = qg_params->theta;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_PHASE, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_s(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 1;
  matrix[1][1].imag = 1;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_S, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_sdg(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 1;
  matrix[1][1].imag = -1;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_SDG, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_t(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 1;
  matrix[1][1].imag = exp((-1 * M_PI) / 4);
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_T, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_tdg(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 1;
  matrix[1][1].imag = exp((-1 * M_PI) / 4);
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_TDG, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_rz(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_zeroed(QG_SINGLE_GATE_SIZE);
  matrix[0][0].imag = exp(-1 * (qg_params->theta / 2));
  matrix[1][1].imag = exp(1j * (qg_params->theta / 2));
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_RZ, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_rx(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_alloc(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = cos(qg_params->theta / 2);
  matrix[0][0].imag = 0;
  matrix[0][1].real = 0;
  matrix[0][1].imag = sin(qg_params->theta / 2);
  matrix[1][0].real = 0;
  matrix[1][0].imag = sin(qg_params->theta / 2);
  matrix[1][1].real = cos(qg_params->theta / 2);
  matrix[1][1].imag = 0;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_RX, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_ry(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_alloc(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = cos(qg_params->theta);
  matrix[0][0].imag = 0;
  matrix[0][1].real = -1 * sin(qg_params->theta / 2);
  matrix[0][1].imag = 0;
  matrix[1][0].real = sin(qg_params->theta / 2);
  matrix[1][0].imag = 0;
  matrix[1][1].real = cos(qg_params->theta / 2);
  matrix[1][1].imag = 0;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_RY, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_sx(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_alloc(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 0.5;
  matrix[0][0].imag = 0.5;
  matrix[0][1].real = 0.5;
  matrix[0][1].imag = -0.5;
  matrix[1][0].real = 0.5;
  matrix[1][0].imag = -0.5;
  matrix[1][1].real = 0.5;
  matrix[1][1].imag = 0.5;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_SX, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_sxdg(struct quantum_gate_params_def* qg_params) {
  if (!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_alloc(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = 0.5;
  matrix[0][0].imag = -0.5;
  matrix[0][1].real = 0.5;
  matrix[0][1].imag = 0.5;
  matrix[1][0].real = 0.5;
  matrix[1][0].imag = 0.5;
  matrix[1][1].real = 0.5;
  matrix[1][1].imag = -0.5;
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_SXDG, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}

struct quantum_gate_def* quantum_gate_u(struct quantum_gate_params_def* qg_params) {
  if(!qg_params) {
    return NULL;
  }
  struct global_matrix_def** matrix = global_matrix_alloc(QG_SINGLE_GATE_SIZE);
  matrix[0][0].real = cos(qg_params->theta / 2);
  matrix[0][0].imag = 0;
  matrix[0][1].real = 0;
  matrix[0][1].imag = -1 * (qg_params->lambda * sin(qg_params->theta / 2));
  matrix[1][0].real = 0;
  matrix[1][0].imag = exp(qg_params->phi) * sin(qg_params->theta / 2);
  matrix[1][1].real = 0;
  matrix[1][1].imag = (qg_params->lambda + qg_params->phi) * cos(qg_params->theta / 2);
  return quantum_gate_init(QG_SINGLE_GATE_SIZE, GLOBAL_GATE_U, GLOBAL_TYPE_SINGLE, matrix, qg_params);
}
