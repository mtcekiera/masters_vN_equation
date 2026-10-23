from pathlib import Path

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm


def plot_error_grid(csv_file, output_file=None, line=False):
    csv_file = Path(csv_file)
    data = pd.read_csv(csv_file)

    # Convert the flat CSV table into a Delta x V grid
    grid = data.pivot(index="V", columns="Delta", values="max_error")

    deltas = grid.columns.to_numpy(dtype=float)
    vs = grid.index.to_numpy(dtype=float)
    errors = grid.to_numpy(dtype=float)

    # LogNorm cannot display exactly zero
    positive_errors = errors[errors > 0]

    if positive_errors.size == 0:
        raise ValueError("All errors are zero.")

    min_positive = np.min(positive_errors)
    plot_errors = np.where(errors > 0, errors, min_positive)

    fig, ax = plt.subplots(figsize=(8, 6))

    mesh = ax.pcolormesh(
        deltas,
        vs,
        plot_errors,
        shading="auto",
        norm=LogNorm(
            vmin=np.min(positive_errors),
            vmax=np.max(positive_errors)
        )
    )

    colorbar = fig.colorbar(mesh, ax=ax)
    colorbar.set_label("Maksymalny błąd bezwzględny")
    if line:
        ax.plot(deltas, deltas * 3.4285714285714266, color="red", linestyle="--", alpha=0.5)
        ax.set_ylim(0.0, 20.1)
        ax.text(6, 17.5, r"$V=3.43\Delta$", color="red", alpha=0.5)

    ax.set_xlabel(r"$\Delta$")
    ax.set_ylabel(r"$V$")
    # ax.set_title(r"Maksymalny błąd RK4 dla $\rho_{11}$")

    fig.tight_layout()

    if output_file is not None:
        output_file = Path(output_file)
        output_file.parent.mkdir(parents=True, exist_ok=True)
        fig.savefig(output_file, dpi=300)

    # plt.show()



import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm
from matplotlib.ticker import LogLocator, LogFormatterMathtext


def plot_error_contour(csv_file, output_file=None):
    data = pd.read_csv(csv_file)

    grid = data.pivot(index="V", columns="Delta", values="max_error")

    deltas = grid.columns.to_numpy(dtype=float)
    vs = grid.index.to_numpy(dtype=float)
    errors = grid.to_numpy(dtype=float)

    D, V = np.meshgrid(deltas, vs)

    positive = errors[errors > 0]
    if positive.size == 0:
        raise ValueError("All errors are zero.")

    min_error = np.min(positive)
    max_error = np.max(positive)

    # Expand limits to complete powers of 10
    vmin = 10 ** np.floor(np.log10(min_error))
    vmax = 10 ** np.ceil(np.log10(max_error))

    if vmin == vmax:
        vmax *= 10

    plot_errors = np.where(errors > 0, errors, vmin)

    levels = np.logspace(
        np.log10(vmin),
        np.log10(vmax),
        30
    )

    fig, ax = plt.subplots(figsize=(8, 6))

    contour = ax.contourf(
        D,
        V,
        plot_errors,
        levels=levels,
        norm=LogNorm(vmin=vmin, vmax=vmax),
        extend="both"
    )

    colorbar = fig.colorbar(contour, ax=ax)

    colorbar.locator = LogLocator(base=10)
    colorbar.formatter = LogFormatterMathtext(base=10)
    colorbar.update_ticks()

    colorbar.set_label("Maksymalny błąd bezwzględny")

    ax.set_xlabel(r"$\Delta$ [au]")
    ax.set_ylabel(r"$V$ [au]")

    fig.tight_layout()

    if output_file is not None:
        fig.savefig(output_file, dpi=300)

    # plt.show()

if __name__ == "__main__":
    plot_error_contour(
        "./data/TEST_a1b1_D1V1/DV_test_p1.csv",
        "./images/TEST_a1b1_D1V1/DV_test_p1.png"
    )
    plot_error_grid(
        "./data/TEST_a08b06_D1V2/DV_test_p1.csv",
        "./images/TEST_a08b06_D1V2/DV_test_p1.png"
    )
    plot_error_grid(
        "./data/TEST_a08b06_D1V2/DV_test_p12.csv",
        "./images/TEST_a08b06_D1V2/DV_test_p12.png"
    )
    plot_error_grid(
        "./data/TEST_a08b06_D1V2/DV_test_p2.csv",
        "./images/TEST_a08b06_D1V2/DV_test_p2.png"
    )

    plot_error_grid(
        "./data/TEST_a08b06_D1V2/DV_test_p1.csv",
        "./images/TEST_a08b06_D1V2/DV_test_p1_line.png",
        True
    )
    plot_error_grid(
        "./data/TEST_a08b06_D1V2/DV_test_p12.csv",
        "./images/TEST_a08b06_D1V2/DV_test_p12_line.png",
        True
    )
    plot_error_grid(
        "./data/TEST_a08b06_D1V2/DV_test_p2.csv",
        "./images/TEST_a08b06_D1V2/DV_test_p2_line.png",
        True
    )