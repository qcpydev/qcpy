class CalculatorInterface:
    def __init__(self, qubits: int, big_endian: bool = False, prep: chr = "z"):
        pass

    def pass_single_gate(self, qubits_to_apply, gate):
        pass

    def pass_custom_gate_queue(self, gate_queue):
        pass

    def pass_multi_gate(self, control: int, target: int, gate):
        pass
