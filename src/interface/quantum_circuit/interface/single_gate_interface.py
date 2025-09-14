class SingleGateInterface:
    def __init__(self, qubits: int, big_endian: bool = False):
        pass

    def __create_gate_queue__(self, qubits_to_apply, gate):
        pass

    def __custom_gate_queue__(self, gate_array):
        pass
