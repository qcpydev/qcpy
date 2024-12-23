from .qlog_lib import qlog_cross, convert_qubits_qlog_append


class QLog:
    def __init__(self, qubits: int) -> None:
        self.qubits = qubits
        self.qlog = qlog_cross.qlog_init(qubits)

    def delete(self) -> None:
        qlog_cross.qlog_delete(self.qlog)
        self.qlog = None

    def end(self) -> int:
        return qlog_cross.qlog_end(self.qlog)

    def append(self, qubits, num_qubits: int, gate_type, gate) -> None:
        qubits = convert_qubits_qlog_append(qubits)
        app_res = qlog_cross.qlog_append(self.qlog, qubits, num_qubits, gate_type, gate)
        if app_res == 1:
            print("insert user error here")
        elif app_res == 2:
            print("insert internal error here")

    def clear(self) -> None:
        qlog_cross.qlog_clear(self.qlog)

    def size(self) -> int:
        return qlog_cross.qlog_size(self.qlog)

    def optimize(self) -> None:
        self.qlog = qlog_cross.qlog_optimize_set(self.qlog)

    def dump(self, verbose: bool = False) -> None:
        qlog_cross.qlog_dump_content(self.qlog, verbose)
        pass
