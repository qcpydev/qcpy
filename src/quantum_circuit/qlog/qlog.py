from .qlog_lib import (
    convert_arr_to_sizes,
    convert_arr_to_str,
    qlog_cross,
    convert_qubits_qlog_append,
    convert_arr_to_int,
)
import numpy as np


class QLog:
    def __init__(self, qubits: int) -> None:
        self.qubits = qubits
        self.qlog = qlog_cross.qlog_init(qubits)

    def __eq__(self, qlog_to_compare) -> bool:
        return qlog_cross.qlog_compare_qlogs(self.qlog, qlog_to_compare)

    def add(self, qlog_to_add):
        self.qlog = qlog_cross.qlog_combine_qlogs(self.qlog, qlog_to_add)

    def equal(self, qlog_to_set) -> None:
        qlog_cross.qlog_delete(self.qlog)
        self.qlog = qlog_to_set

    def delete(self) -> None:
        qlog_cross.qlog_delete(self.qlog)
        self.qlog = None

    def end(self) -> int:
        return qlog_cross.qlog_end(self.qlog)

    def append(
        self,
        qubits,
        num_qubits: int,
        gate_type: int,
        gate: int,
        theta: float,
        phi: float,
        lmbda: float,
    ) -> None:
        qubits = convert_qubits_qlog_append(qubits)
        app_res = qlog_cross.qlog_append(
            self.qlog, qubits, num_qubits, gate_type, gate, theta, phi, lmbda
        )
        if app_res == 1:
            print("insert user error")
        elif app_res == 2:
            print("insert internal error")
            self.dump(verbose=True)

    def append_custom(
        self, qubits, num_qubits: int, gate_type: int, gate: np.array
    ) -> None:
        pass

    def clear(self) -> None:
        qlog_cross.qlog_clear(self.qlog)

    def size(self) -> int:
        return qlog_cross.qlog_size(self.qlog)

    def optimize(self) -> None:
        #self.qlog = qlog_cross.qlog_optimize_set(self.qlog)
        pass

    def dump(self, verbose: bool = False) -> None:
        qlog_cross.qlog_dump_content(self.qlog, verbose)

    def get_names(self):
        out = qlog_cross.qlog_get_gate_names(self.qlog)
        return convert_arr_to_str(out, self.size())

    def get_types(self):
        out = qlog_cross.qlog_get_gate_types(self.qlog)
        return convert_arr_to_str(out, self.size())

    def get_qubits(self):
        out = qlog_cross.qlog_get_gate_qubits(self.qlog)
        return convert_arr_to_int(out, self.size(), self.get_sizes())

    def get_sizes(self):
        return convert_arr_to_sizes(
            qlog_cross.qlog_get_entry_sizes(self.qlog), self.size()
        )
