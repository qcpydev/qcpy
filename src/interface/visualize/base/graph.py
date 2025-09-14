import matplotlib.pyplot as plt


def graph(_text, _background, num_qubits):
    plt.clf()
    plt.close()
    fig, ax = plt.subplots(figsize=(num_qubits + 3, num_qubits + 3))
    plt.setp(ax.get_xticklabels(), rotation=75, ha="right", color=_text)
    plt.setp(ax.get_yticklabels(), color=_text)
    ax.spines["bottom"].set_color(_text)
    ax.spines["top"].set_color(_background)
    ax.spines["right"].set_color(_background)
    ax.spines["left"].set_color(_text)
    ax.tick_params(axis="x", colors=_text)
    ax.tick_params(axis="y", colors=_text)
    ax.set_facecolor(_background)
    fig.patch.set_facecolor(_background)
    return ax
