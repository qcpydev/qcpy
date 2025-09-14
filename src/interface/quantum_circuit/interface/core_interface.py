class CoreInterface:
    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        pass

    def __operator_matrix__(self, gate_queue):
        pass

    def __multi_operator_matrix__(self, multi_gate_queue):
        pass
