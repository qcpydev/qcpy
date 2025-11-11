def multi_control(is_connector: bool = False, is_end: bool = False) -> str:
    """Formats a controlled section of the drawing
    Args:
        is_connector (bool): Determines if this is not the bottom or top of a drawing.
        is_end (int): Determines if the connector is at the end of a multi drawing.
    Returns:
        str: Formatted version of a multi controlled drawing.
    """
    res = "     ──■──  │  "
    if is_connector:
        res = "  │  ──■──  │  "
    elif is_end:
        res = "  │  ──■──     "
    return res


def multi_connect() -> str:
    """Shows the drawing of when a qubit is simply passed through in the logic
    Returns:
        str: A connect drawing.
    """
    return "  │  ──┼──  │  "


def horizontal_line() -> str:
    """A simple horizontal line in the circuit drawing
    Returns:
        str: A horizontal line block.
    """
    return "     ─────     "


def block_bottom() -> str:
    """When a block has ended this is called in circuit drawing.
    Returns:
        str: End of the block drawing.
    """
    return "│   │┤   ├└───┘"


def block_connect() -> str:
    """When a qubit is in range of a block
    Returns:
        str: A connector for a block drawing.
    """
    return "│   │┤   ├│   │"


def block_gate(gate: str) -> str:
    """The "center" of a block drawing
    Args:
        gate (str): Not currently used (Needs to change that!)
    Returns:
        str: Center block drawing.
    """
    return "│   │┤MUL├│   │"


def block_top() -> str:
    """The start of the range for a block drawing
    Returns:
        str: Top of a block drawing.
    """
    return "┌───┐┤   ├│   │"


def single_gate(gate: str, is_controlled: bool = False, is_start: bool = False) -> str:
    """Draws a gate with it's symbol inside.
    Args:
        gate (str): String char representation of a quantum gate.
        is_controlled (bool): Determines if the gate is controlled by another qubit.
        is_start (bool): Determines if this gate is upside down with a target qubit.
    Returns:
        str: A quantum gate to then be inserted into a circuit drawing.
    """
    top = "┌─┴─┐" if (is_controlled and not is_start) else "┌───┐"
    middle = "┤"
    if len(gate) == 1:
        middle += " " + gate + " ├"
    elif len(gate) == 2:
        middle += gate + " ├"
    else:
        middle += gate + "├"
    return top + middle + ("└───┘" if not is_start else "└─┬─┘")


def swap_point() -> str:
    """Block drawing of a swap gate.
    Returns:
        str: Swap gate drawing.
    """
    return "     ──╳──     "


def vertical_line() -> str:
    """A simple vertical line block.
    Returns:
        str: A string of a vertical line block.
    """
    return "       │       "
