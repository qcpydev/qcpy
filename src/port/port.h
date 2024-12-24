#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "port_stats.h"
#include "../quantum_circuit/qlog/qlog.h"



struct matrix_def import_matrix();
struct matrix_def export_matrix();

struct matrix_def get_quantum_gate();
struct matrix_def send_quantum_gate();

