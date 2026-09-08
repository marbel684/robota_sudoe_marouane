#!/usr/bin/env python3

import json
from pathlib import Path

import numpy as np


def load_demonstration(path):
    """Charge une démonstration JSON v1."""
    return json.loads(Path(path).read_text())


def resample_times(data, period=0.02):
    """Retourne les temps uniformément échantillonnés.

    Cette première version conserve les échantillons et prépare
    l'interface pour une interpolation future des poses.
    """
    samples = data["samples"]
    if not samples:
        return data

    t0 = samples[0]["t"]
    times = np.array([s["t"] - t0 for s in samples])

    if len(times) < 2:
        return data

    duration = times[-1]
    new_times = np.arange(0.0, duration + period, period)

    data["resampled_times"] = new_times.tolist()
    data["duration"] = float(duration)
    return data
