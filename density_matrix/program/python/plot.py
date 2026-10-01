import pandas as pd
import matplotlib.pyplot as plt


def plot_evolution_comparison(
    theory_file,
    numeric_file,
    output_file,
    a,
    b,
    delta,
    V
):
    # ---------------------------------------------------------
    # LOAD DATA
    # ---------------------------------------------------------

    theory = pd.read_csv(theory_file)
    numeric = pd.read_csv(numeric_file)


    # ---------------------------------------------------------
    # CREATE FIGURE
    # ---------------------------------------------------------

    fig, axes = plt.subplots(
        3,
        2,
        figsize=(10, 10),
        sharex="col"
    )


    # =========================================================
    # rho_11
    # =========================================================

    axes[0, 0].plot(
        theory["t"],
        theory["p1_real"]
    )

    axes[0, 0].set_title(
        r"Theory: $\rho_{11}$"
    )


    axes[0, 1].plot(
        numeric["t"],
        numeric["p1_real"]
    )

    axes[0, 1].set_title(
        r"RK4: $\rho_{11}$"
    )


    # =========================================================
    # rho_22
    # =========================================================

    axes[1, 0].plot(
        theory["t"],
        theory["p2_real"]
    )

    axes[1, 0].set_title(
        r"Theory: $\rho_{22}$"
    )


    axes[1, 1].plot(
        numeric["t"],
        numeric["p2_real"]
    )

    axes[1, 1].set_title(
        r"RK4: $\rho_{22}$"
    )


    # =========================================================
    # rho_12
    # =========================================================

    axes[2, 0].plot(
        theory["t"],
        theory["p12_real"],
        label="Re"
    )

    axes[2, 0].plot(
        theory["t"],
        theory["p12_imag"],
        label="Im"
    )

    axes[2, 0].set_title(
        r"Theory: $\rho_{12}$"
    )

    axes[2, 0].legend()


    axes[2, 1].plot(
        numeric["t"],
        numeric["p12_real"],
        label="Re"
    )

    axes[2, 1].plot(
        numeric["t"],
        numeric["p12_imag"],
        label="Im"
    )

    axes[2, 1].set_title(
        r"RK4: $\rho_{12}$"
    )

    axes[2, 1].legend()


    # =========================================================
    # FORMATTING
    # =========================================================

    for ax in axes.flat:
        ax.grid(alpha=0.3)


    axes[2, 0].set_xlabel("t [au]")
    axes[2, 1].set_xlabel("t [au]")


    # Same y-scale within each theory / RK4 pair
    for row in range(3):

        ymin = min(
            axes[row, 0].get_ylim()[0],
            axes[row, 1].get_ylim()[0]
        )

        ymax = max(
            axes[row, 0].get_ylim()[1],
            axes[row, 1].get_ylim()[1]
        )

        axes[row, 0].set_ylim(ymin, ymax)
        axes[row, 1].set_ylim(ymin, ymax)


    # =========================================================
    # MAIN TITLE
    # =========================================================

    if a == b:

        fig.suptitle(
            rf"$a=b$, "
            rf"$\Delta={delta}$, $V={V}$",
            fontsize=14
        )

    else:

        fig.suptitle(
            rf"$a={a}$, $b={b}$, "
            rf"$\Delta={delta}$, $V={V}$",
            fontsize=14
        )


    fig.tight_layout()

    plt.savefig(
        output_file,
        dpi=300
    )

    plt.show()


# =============================================================
# EXAMPLE
# =============================================================

if __name__ == "__main__":

    plot_evolution_comparison(
        theory_file="data/th_a1b0E1V0.csv",
        numeric_file="data/num_a1b0E1V0.csv",
        output_file="images/comparison.png",

        a=1,
        b=0,

        delta=1,
        V=0
    )