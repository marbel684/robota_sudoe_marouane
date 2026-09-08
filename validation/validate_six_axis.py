#!/usr/bin/env python3
"""
Validation hors robot réel de l'admittance 6 axes et du Jacobien UR5e.

Le script :
- vérifie les unités des paramètres ;
- simule translation et rotation ;
- vérifie les dead-zones ;
- vérifie les limites vitesse/accélération ;
- vérifie la stabilité des six axes ;
- calcule un Jacobien UR5e avec les paramètres DH standard ;
- évalue J J# v et les valeurs singulières ;
- teste plusieurs configurations pour détecter les zones proches
  de singularité.

Aucune commande robot n'est envoyée.
"""

from dataclasses import dataclass
import math
import numpy as np


@dataclass
class P:
    M_t: float = 12.0
    M_r: float = 0.10
    B_t: float = 8.31
    B_r: float = 0.76
    f_alpha: float = 0.08
    v_alpha: float = 0.25
    dead_f: float = 1.5
    dead_t: float = 0.08
    freq: float = 250.0
    vmax: float = 0.20
    wmax: float = 0.35
    qdotmax: float = 0.60
    amax: float = 0.50
    alphamax: float = 0.80
    lambda_dls: float = 0.02


def dead_zone_vector(x, threshold):
    n = np.linalg.norm(x)
    if n <= threshold or n < 1e-12:
        return np.zeros_like(x)
    return x * ((n - threshold) / n)


def dh(a, alpha, d, theta):
    ca, sa = math.cos(alpha), math.sin(alpha)
    ct, st = math.cos(theta), math.sin(theta)
    return np.array([
        [ct, -st * ca, st * sa, a * ct],
        [st, ct * ca, -ct * sa, a * st],
        [0.0, sa, ca, d],
        [0.0, 0.0, 0.0, 1.0],
    ])


def ur5e_jacobian(q):
    # UR5/UR5e nominal DH parameters.
    # The exact calibrated robot model should ultimately come from
    # robot_description on the real system.
    a = [
        0.0,
        -0.42500,
        -0.39225,
        0.0,
        0.0,
        0.0,
    ]
    d = [
        0.16250,
        0.0,
        0.0,
        0.13330,
        0.09970,
        0.09960,
    ]
    alpha = [
        math.pi / 2,
        0.0,
        0.0,
        math.pi / 2,
        -math.pi / 2,
        0.0,
    ]

    T = np.eye(4)
    origins = [T[:3, 3].copy()]
    axes = [T[:3, 2].copy()]

    for i in range(6):
        T = T @ dh(a[i], alpha[i], d[i], q[i])
        origins.append(T[:3, 3].copy())
        axes.append(T[:3, 2].copy())

    o_n = origins[-1]
    Jv = np.zeros((3, 6))
    Jw = np.zeros((3, 6))

    for i in range(6):
        Jv[:, i] = np.cross(axes[i], o_n - origins[i])
        Jw[:, i] = axes[i]

    return np.vstack((Jv, Jw))


def dls_pinv(J, lam):
    U, S, Vt = np.linalg.svd(J, full_matrices=False)
    Sd = S / (S * S + lam * lam)
    return Vt.T @ np.diag(Sd) @ U.T


def validate_jacobian(p):
    configurations = [
        np.zeros(6),
        np.array([0.0, -math.pi / 2, 0.0, -math.pi / 2, 0.0, 0.0]),
        np.array([0.4, -1.0, 1.2, -1.3, 0.8, 0.3]),
        np.array([-1.0, -1.2, 1.0, -1.1, 1.0, -0.5]),
    ]

    v = np.array([0.05, -0.03, 0.02, 0.08, -0.04, 0.03])

    print("--- Jacobien UR5e ---")

    worst_error = 0.0
    worst_condition = 0.0

    for i, q in enumerate(configurations):
        J = ur5e_jacobian(q)
        Jp = dls_pinv(J, p.lambda_dls)

        reconstructed = J @ Jp @ v
        error = np.linalg.norm(reconstructed - v)

        S = np.linalg.svd(J, compute_uv=False)
        sigma_min = float(np.min(S))
        sigma_max = float(np.max(S))
        condition = sigma_max / max(sigma_min, 1e-12)

        worst_error = max(worst_error, error)
        worst_condition = max(worst_condition, condition)

        print(
            f"config {i}: "
            f"sigma_min={sigma_min:.6e}, "
            f"condition={condition:.3e}, "
            f"||JJ#v-v||={error:.6e}"
        )

    print(f"worst reconstruction error = {worst_error:.6e}")
    print(f"worst condition indicator = {worst_condition:.3e}")


def validate_dynamics(p):
    dt = 1.0 / p.freq
    print()
    print("--- Dynamique 6 axes ---")
    print(f"dt = {dt:.6f} s")
    print(f"max dv translation = {p.amax * dt:.6f} m/s")
    print(f"max dw rotation = {p.alphamax * dt:.6f} rad/s")

    # Continuous first-order poles of M*v_dot + B*v = F.
    p_t = -p.B_t / p.M_t
    p_r = -p.B_r / p.M_r

    print(f"translation pole = {p_t:.6f} 1/s")
    print(f"rotation pole = {p_r:.6f} 1/s")

    assert p_t < 0.0
    assert p_r < 0.0

    # Force/torque steps.
    force = np.array([5.0, 0.0, 0.0])
    torque = np.array([0.15, 0.0, 0.0])

    force_eff = dead_zone_vector(force, p.dead_f)
    torque_eff = dead_zone_vector(torque, p.dead_t)

    v_ss = force_eff / p.B_t
    w_ss = torque_eff / p.B_r

    print(f"5 N effective force = {np.linalg.norm(force_eff):.4f} N")
    print(f"theoretical translational steady speed = {np.linalg.norm(v_ss):.6f} m/s")
    print(f"0.15 Nm effective torque = {np.linalg.norm(torque_eff):.4f} Nm")
    print(f"theoretical angular steady speed = {np.linalg.norm(w_ss):.6f} rad/s")

    assert np.isfinite(v_ss).all()
    assert np.isfinite(w_ss).all()


def validate_saturation(p):
    dt = 1.0 / p.freq
    print()
    print("--- Saturations ---")

    v = 0.0
    previous = 0.0
    max_observed_acc = 0.0

    for _ in range(500):
        requested = 2.0
        max_dv = p.amax * dt
        dv = np.clip(requested - previous, -max_dv, max_dv)
        v = previous + dv
        v = np.clip(v, -p.vmax, p.vmax)

        observed = abs((v - previous) / dt)
        max_observed_acc = max(max_observed_acc, observed)
        previous = v

    print(f"max observed acceleration = {max_observed_acc:.6f} m/s^2")
    print(f"limit = {p.amax:.6f} m/s^2")
    print(f"final speed = {v:.6f} m/s")
    assert max_observed_acc <= p.amax + 1e-10
    assert abs(v) <= p.vmax + 1e-12


def main():
    p = P()

    assert p.M_t > 0 and p.M_r > 0
    assert p.B_t > 0 and p.B_r > 0
    assert 0 < p.f_alpha <= 1
    assert 0 < p.v_alpha <= 1
    assert p.freq > 0
    assert p.lambda_dls > 0

    validate_dynamics(p)
    validate_saturation(p)
    validate_jacobian(p)

    print()
    print("VALIDATION 6 AXES TERMINEE")
    print("Aucun mouvement robot n'a été commandé.")


if __name__ == "__main__":
    main()
