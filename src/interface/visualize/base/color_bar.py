import numpy as np


def color_bar(plt, _text, _accent, colors, norm) -> None:
    cbar = plt.colorbar(
        plt.cm.ScalarMappable(cmap=colors, norm=norm), ax=plt.gca(), shrink=0.55
    )
    cbar.set_label("Phase Angle", rotation=270, labelpad=15, color=_accent)
    cbar.set_ticks([2 * np.pi, (3 * np.pi) / 2, np.pi, np.pi / 2, 0])
    cbar.ax.yaxis.set_tick_params(color=_text)
    cbar.outline.set_edgecolor(_text)
    cbar.set_ticklabels(["2π", "3π / 2", "π", "π / 2", "0"], color=_text)
    return
