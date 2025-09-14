import numpy as np


def convert_state(to_convert) -> np.array:
    from ... import quantumcircuit

    if isinstance(to_convert, quantumcircuit):
        to_convert = to_convert.state

    to_convert = to_convert.flatten()
    return np.around(to_convert, int(np.log2(to_convert.size)))
