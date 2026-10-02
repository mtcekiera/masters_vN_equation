from pathlib import Path

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


DATA_ROOT = Path("./data")
IMAGE_ROOT = Path("./images")


def get_observable(data, observable):
    if observable == "p1":
        return data["p1_real"].to_numpy()

    if observable == "p2":
        return data["p2_real"].to_numpy()

    if observable == "p12":
        real = data["p12_real"].to_numpy()
        imag = data["p12_imag"].to_numpy()
        return real + 1j * imag

    raise ValueError(f"Unknown observable '{observable}'. Choose 'p1', 'p2', or 'p12'.")


def max_error(theory, numeric, observable):
    y_th = get_observable(theory, observable)
    y_num = get_observable(numeric, observable)

    return np.max(np.abs(y_th - y_num))


def analyse_convergence(data_dir, observable):
    manifest = pd.read_csv(data_dir / "manifest.csv")

    omega_dts = []
    errors = []

    for _, run in manifest.iterrows():
        theory = pd.read_csv(data_dir / run["theory_file"])
        numeric = pd.read_csv(data_dir / run["numeric_file"])

        omega = np.sqrt(run["delta"]**2 + run["v"]**2)
        omega_dt = omega * run["dt"]

        omega_dts.append(omega_dt)
        errors.append(max_error(theory, numeric, observable))

    omega_dts = np.asarray(omega_dts)
    errors = np.asarray(errors)

    order = np.argsort(omega_dts)
    return omega_dts[order], errors[order]


def convergence_order(omega_dts, errors):
    mask = (omega_dts > 0) & (errors > 0) & np.isfinite(errors)

    slope, intercept = np.polyfit(
        np.log(omega_dts[mask]),
        np.log(errors[mask]),
        1
    )

    return slope, intercept


def plot_convergence(omega_dts, errors, observable, output_file):
    slope, intercept = convergence_order(omega_dts, errors)

    plt.figure(figsize=(7, 5))


    x_fit = np.logspace(
        np.log10(np.min(omega_dts)),
        np.log10(np.max(omega_dts)),
        200
    )

    y_fit = np.exp(intercept) * x_fit**slope

    plt.loglog(
        x_fit,
        y_fit,
        "--",
        label=rf"Dopasowanie: $E \propto (\omega\Delta t)^{{{slope:.2f}}}$"
    )


    plt.loglog(
        omega_dts,
        errors,
        "x",
        label="Wyniki błędu",
        color="black"
    )

    plt.xlabel(r"$\omega \Delta t$")
    plt.ylabel("Maksymalny błąd bezwzględny")
    # plt.title(f"RK4 convergence: {observable}")

    plt.text(
        0.05,
        0.95,
        f"nachylenie w skali log-log ≈ {slope:.3f}",
        transform=plt.gca().transAxes,
        verticalalignment="top"
    )

    plt.grid(True, which="both", alpha=0.3)
    plt.legend()
    plt.tight_layout()

    plt.savefig(output_file, dpi=300)
    plt.close()

    print(f"{observable}: slope = {slope:.4f}")


def analyse_folder(folder_name):
    data_dir = DATA_ROOT / folder_name
    image_dir = IMAGE_ROOT / folder_name

    if not data_dir.exists():
        raise FileNotFoundError(f"Data folder does not exist: {data_dir}")

    if not (data_dir / "manifest.csv").exists():
        raise FileNotFoundError(f"Manifest not found in: {data_dir}")

    image_dir.mkdir(parents=True, exist_ok=True)

    for observable in ["p1", "p2", "p12"]:
        omega_dts, errors = analyse_convergence(data_dir, observable)

        output_file = image_dir / f"{observable}.png"

        plot_convergence(
            omega_dts,
            errors,
            observable,
            output_file
        )


if __name__ == "__main__":
    folder_name = "a1b1E1V2"

    analyse_folder(folder_name)