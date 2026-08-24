#include <assert.h>
#include <qlog_entry.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef qlog_entry_t *(*qlog_entry_known_gate_func)(qlog_entry_init_params_t *);

const qlog_entry_known_gate_func qlog_entry_known_gate_funcs[GATE_MAX] = {
    [GATE_HADAMARD] = qlog_entry_init_hadamard_gate,
    [GATE_T] = qlog_entry_init_t_gate,
    [GATE_TDG] = qlog_entry_init_tdg_gate,
    [GATE_CX] = qlog_entry_init_cx_gate,
};

qlog_entry_t *qlog_entry_init(uint64_t id, block_t block) {
  qlog_entry_t *qlog_entry = (qlog_entry_t *)malloc(sizeof(qlog_entry_t));

  qlog_entry->entry_id = id;
  qlog_entry->qubit_bitmask = block.qubit_bitmask;
  qlog_entry->controlled_bitmask = block.controlled_bitmask;
  qlog_entry->target_bitmask = block.target_bitmask;

  qlog_entry->controlled_bitpack = block.controlled_bitpack;
  qlog_entry->target_bitpack = block.target_bitpack;
  qlog_entry->theta = block.theta;
  qlog_entry->phi = block.phi;
  qlog_entry->lambda = block.lmbda;

  qlog_entry->controlled_count = block.controlled_count;
  qlog_entry->target_count = block.target_count;

  qlog_entry->gate_name = (base_gate_e)block.gate;
  qlog_entry->gate_type = (base_type_e)block.type;
  qlog_entry->qubit_count = block.qubits;

  qubit_t qubit_min = QLOG_MAX_QUBITS;
  qubit_t qubit_max = 0;

  bitmask_t i = 1;
  qubit_t qubit = 0;
  while (i < BLOCK_BITMASK_MAX) {
    if (i & qlog_entry->qubit_bitmask) {
      qubit_min = QLOG_ENTRY_MIN_QUBIT(qubit_min, qubit);
      qubit_max = QLOG_ENTRY_MAX_QUBIT(qubit_max, qubit);
    }

    qubit += 1;
    i <<= 1;
  }

  qlog_entry->min_qubit = qubit_min;
  qlog_entry->max_qubit = qubit_max;

  qlog_entry->inverted = block.inverted;

  return qlog_entry;
}

uint64_t qlog_entry_qubit_set_flags(uint8_t *qubits, uint8_t count) {
  uint64_t qubit_int = 0;

  for (uint8_t i = 0; i < count; ++i) {
    qubit_int |= (1 << qubits[i]);
  }

  if (!qubit_int) {
  }

  return qubit_int;
}

uint8_t *qlog_entry_deconstruct_qubits(qlog_entry_t *qlog_entry) {
  if (!qlog_entry) {
    // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, qlog_entry, QLOG_ENTRY_NULL);
  }

  uint8_t *qubit_decon =
      (uint8_t *)malloc(sizeof(uint8_t) * qlog_entry->qubit_count);

  if (!qubit_decon) {
    // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, qubit_decon, QLOG_ENTRY_NULL_QUBITS);
  }

  uint64_t qubit = qlog_entry->inverted ? 1ULL << 62 : 1;
  uint16_t i = 0;

  do {
    if (qubit & qlog_entry->qubit_bitmask) {
      // qubit_decon[i] = (uint8_t)log2(qubit);
      ++i;
    }

    if (qlog_entry->inverted) {
      qubit >>= 1;
    } else {
      qubit <<= 1;
    }

  } while (qubit > 0 && qubit <= 1ULL << 63);

  return qubit_decon;
}

void qlog_entry_delete(qlog_entry_t *qlog_entry) {
  if (!qlog_entry) {
    // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, qlog_entry, QLOG_ENTRY_NULL);
  }

  // free(qlog_entry->gate_params);
  // free(qlog_entry->stats);
  free(qlog_entry);

  qlog_entry = NULL;

  return;
}

base_gate_e qlog_entry_qg_name(qlog_entry_t *qlog_entry) {
  return qlog_entry->gate_name;
}

base_type_e qlog_entry_qg_type(qlog_entry_t *qlog_entry) {
  return qlog_entry->gate_type;
}

void qlog_entry_dump_content(qlog_entry_t *qlog_entry, bool verbose) {
  if (!qlog_entry) {
    // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, qlog_entry, QLOG_ENTRY_NULL);
  }

  printf("(");
  if (verbose) {
    printf("Qubit Count: %d", qlog_entry->qubit_count);
  }

  printf(" on: [");
  uint8_t *qubits = qlog_entry_deconstruct_qubits(qlog_entry);

  for (uint8_t i = 0; i < qlog_entry->qubit_count; ++i) {
    printf(" %d ", qubits[i]);
    if (i + 1 < qlog_entry->qubit_count) {
      printf(",");
    }
  }

  printf("])\n");
  return;
}

qlog_entry_t *qlog_entry_duplicates_to_clean(qlog_entry_t *qlog_entry) {
  if (!qlog_entry || qlog_entry->prev_entry) {
    return NULL;
  }

  if (!qlog_entry_compare(qlog_entry, qlog_entry->prev_entry)) {
    return qlog_entry;
  }

  uint64_t qlog_entry_count = 1;
  qlog_entry_t *prev = qlog_entry->prev_entry;

  while (prev->prev_entry && qlog_entry_compare(prev, prev->prev_entry)) {
    ++qlog_entry_count;
    prev = prev->prev_entry;
  }

  if (qlog_entry_count & 1) {
    qlog_entry_t *next = qlog_entry->next_entry;

    if (!prev->next_entry || !prev->next_entry->next_entry) {
    }

    prev->next_entry->next_entry->next_entry = prev;
    prev->next_entry = prev->next_entry->next_entry;
    qlog_entry_delete(next);

    if (!next) {
      // QCPY_ERROR(QCPY_ERROR_QLOG_ENTRY, next, QLOG_ENTRY_NULL);
    }
  }

  return prev;
}

const char *get_qlog_entry_gate(qlog_entry_t *qlog_entry) {
  assert(qlog_entry);
  if (qlog_entry) {
  }

  return NULL;
}

const char *get_qlog_entry_gate_type(qlog_entry_t *qlog_entry) {
  assert(qlog_entry);
  if (qlog_entry) {
  }

  return NULL;
}

bool qlog_entry_compare(qlog_entry_t *qlog_entry, qlog_entry_t *to_compare) {
  if (!qlog_entry || !to_compare) {
    return false;
  }

  return qlog_entry->qubit_count == to_compare->qubit_count &&
         qlog_entry->gate_type == to_compare->gate_type &&
         qlog_entry->gate_name == to_compare->gate_name &&
         qlog_entry->qubit_bitmask == to_compare->qubit_bitmask;
}

qlog_entry_t *
qlog_entry_init_known_gate(qlog_entry_init_params_t *qlog_params) {
  assert(qlog_params);

  qlog_entry_t *qlog_entry = (qlog_entry_t *)malloc(sizeof(qlog_entry_t));
  assert(qlog_entry);

  qlog_entry->qubit_bitmask =
      base_create_qubit_bitmask(qlog_params->qubits, qlog_entry->qubit_count);

  qlog_entry->theta = qlog_params->theta;
  qlog_entry->phi = qlog_params->phi;
  qlog_entry->lambda = qlog_params->lambda;
  qlog_entry->gate_name = qlog_params->gate_name;
  qlog_entry->gate_type = qlog_params->gate_type;
  qlog_entry->qubit_count = qlog_params->qubit_count;
  qlog_entry->controlled_count = qlog_params->controlled_count;
  qlog_entry->target_count = qlog_params->target_count;
  qlog_entry->inverted = qlog_params->inverted;

  return qlog_entry;
}

static qlog_entry_t *qlog_entry_init_base(qubit_t *qubits, qubit_t qubit_count,
                                          base_gate_e gate, base_type_e type) {
  qlog_entry_t *qlog_entry = (qlog_entry_t *)malloc(sizeof(qlog_entry_t));
  memset(qlog_entry, 0, sizeof(qlog_entry_t));
  assert(qlog_entry);

  qlog_entry->qubit_bitmask = base_create_qubit_bitmask(qubits, qubit_count);
  qlog_entry->gate_name = gate;
  qlog_entry->gate_type = type;
  qlog_entry->entry_id = QLOG_ENTRY_DUMMY_ID;
  qlog_entry->qubit_count = qubit_count;

  return qlog_entry;
}

qlog_entry_t *
qlog_entry_init_hadamard_gate(qlog_entry_init_params_t *qlog_params) {
  assert(qlog_params);

  qlog_entry_t *qlog_entry =
      qlog_entry_init_base(qlog_params->qubits, qlog_params->qubit_count,
                           GATE_HADAMARD, TYPE_SINGLE);

  return qlog_entry;
}

qlog_entry_t *qlog_entry_init_t_gate(qlog_entry_init_params_t *qlog_params) {
  assert(qlog_params);

  qlog_entry_t *qlog_entry = qlog_entry_init_base(
      qlog_params->qubits, qlog_params->qubit_count, GATE_T, TYPE_SINGLE);

  return qlog_entry;
}

qlog_entry_t *qlog_entry_init_tdg_gate(qlog_entry_init_params_t *qlog_params) {
  assert(qlog_params);
  qlog_entry_t *qlog_entry = qlog_entry_init_base(
      qlog_params->qubits, qlog_params->qubit_count, GATE_TDG, TYPE_SINGLE);
  return qlog_entry;
}
qlog_entry_t *qlog_entry_init_rz_gate(qlog_entry_init_params_t *qlog_params) {
  assert(qlog_params);

  qlog_entry_t *qlog_entry = qlog_entry_init_base(
      qlog_params->qubits, qlog_params->qubit_count, GATE_RZ, TYPE_SINGLE);

  qlog_entry->theta = qlog_params->theta;

  return qlog_entry;
}

qlog_entry_t *qlog_entry_init_rx_gate(qlog_entry_init_params_t *qlog_params) {
  assert(qlog_params);

  qlog_entry_t *qlog_entry = qlog_entry_init_base(
      qlog_params->qubits, qlog_params->qubit_count, GATE_RX, TYPE_SINGLE);

  qlog_entry->theta = qlog_params->theta;

  return qlog_entry;
}

qlog_entry_t *qlog_entry_init_cx_gate(qlog_entry_init_params_t *qlog_params) {
  assert(qlog_params);

  qlog_entry_t *qlog_entry = qlog_entry_init_base(
      qlog_params->qubits, qlog_params->qubit_count, GATE_CX, TYPE_CONTROLLED);

  qlog_entry->controlled_bitmask = base_create_qubit_bitmask(
      qlog_params->controls, qlog_params->controlled_count);

  qlog_entry->controlled_bitpack = base_create_qubit_bitpack(
      qlog_params->controls, qlog_params->controlled_count);

  qlog_entry->target_bitmask = base_create_qubit_bitmask(
      qlog_params->targets, qlog_params->target_count);

  qlog_entry->target_bitpack = base_create_qubit_bitpack(
      qlog_params->targets, qlog_params->target_count);

  return qlog_entry;
}
