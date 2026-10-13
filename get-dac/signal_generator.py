import numpy as np
import time


def get_sin_wave_amplitude(freq, time):
    return (np.sin(2 * np.pi * freq * time) + 1) / 2

def get_triangle_wave_amplitude(freq, time):
    phase = (freq*time)%1
    return 2 * phase if phase > 0.5 else (1 - phase)


def wait_for_sampling_period(sampling_frequency):
    time.sleep(1 / sampling_frequency)