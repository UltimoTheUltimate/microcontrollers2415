import numpy as np
import matplotlib.pyplot as plt


def goertzel_algorithm(x, f_k, f_s):
    # Compute the magnitude of frequency f_k in signal x using the Goertzel algorithm
    N = len(x)
    omega = 2 * np.pi * f_k / f_s
    coeff = 2 * np.cos(omega)

    s1 = 0
    s2 = 0

    for n in range(N):
        s0 = x[n] + coeff * s1 - s2
        s2 = s1
        s1 = s0

    magnitude = np.sqrt(s1 ** 2 + s2 ** 2 - coeff * s1 * s2)
    return magnitude


# Define parameters
fs = 8000                                                   # Sampling frequency (Hz)
dot_duration = 0.1                                          # Dot duration (seconds)
dash_duration = 3 * dot_duration                            # Dash duration (seconds)
pause_symbol_duration = dot_duration                        # Pause duration must be equal to dot duration (unit) for
                                                            # symbol forming same character (i.e. 3 dashes making
                                                            # an "O," so there's one pause of this unit length
                                                            # between each dash) (seconds)
pause_char_duration = 3 * pause_symbol_duration             # Pause duration for new character in Morse code within
                                                            # the same word
pause_word_duration = 7 * pause_symbol_duration             # Pause duration for beginning of a new Morse code word
frame_size = 0.01                                           # Frame duration (10 ms)
N_dot = int(fs * dot_duration)                              # Samples for dot
N_dash = int(fs * dash_duration)                            # Samples for dash
N_pause_symbol_duration = int(fs * pause_symbol_duration)   # Samples for symbol pause
N_pause_char_duration = int(fs * pause_char_duration)       # Samples for character pause
N_pause_word_duration = int(fs * pause_word_duration)       # Samples for word pause
N_frame = int(fs * frame_size)                              # Samples per frame

# Signals to enerate Morse sequence
t_dot = np.arange(N_dot) / fs
t_dash = np.arange(N_dash) / fs
t_pause_symbol_duration = np.arange(N_pause_symbol_duration) / fs
t_pause_char_duration = np.arange(N_pause_char_duration) / fs
t_pause_word_duration = np.arange(N_pause_word_duration) / fs
dot_signal = np.sin(2 * np.pi * 700 * t_dot)                        # 700 Hz tone for dot
dash_signal: object = np.sin(2 * np.pi * 700 * t_dash)              # 700 Hz tone for dash
pause_symbol_signal = np.zeros(N_pause_symbol_duration)             # Unit pause
pause_char_signal = np.zeros(N_pause_char_duration)                 # Char pause
pause_word_signal = np.zeros(N_pause_word_duration)                 # Word pause

# Concatenate dots, dashes, and pauses to form test signal ("AO" in Morse code, in this case)
morse_signal = np.concatenate([dot_signal, pause_symbol_signal, dash_signal, pause_char_signal,
                               dash_signal, pause_symbol_signal, dash_signal, pause_symbol_signal, dash_signal])
t_total = np.arange(len(morse_signal)) / fs  # Full time array

# Split into frames and detect target (700 Hz) tone
binary_output = []
time_stamps = np.arange(0, len(morse_signal) / fs, frame_size)  # Time for each frame

threshold = 35  # Set a magnitude threshold (adjustable)

for i in range(0, len(morse_signal), N_frame):
    frame = morse_signal[i:i + N_frame]
    if len(frame) < N_frame:
        frame = np.pad(frame, (0, N_frame - len(frame)), 'constant')

    magnitude_700 = goertzel_algorithm(frame, 700, fs)
    print(f"Frame {i // N_frame}: Magnitude = {magnitude_700}")

    binary_value = 1 if magnitude_700 > threshold else 0
    binary_output.append(binary_value)

# Ensure the binary signal aligns with time
binary_output.append(binary_output[-1])  # Extend the last value for the plot

# Plot the Morse sequence and detected digital signal
plt.figure(figsize=(10, 4))

# Plot the original analog Morse signal
plt.plot(t_total, morse_signal, label="Analog Morse Signal (700 Hz)", alpha=0.7)

# Plot the digital signal as a step function
plt.step(time_stamps, binary_output[:-1], where='post', label="Detected Digital Signal (1 = 700 Hz)", linewidth=2,
         color='red')

plt.xlabel("Time (seconds)")
plt.ylabel("Amplitude / Digital Output")
plt.title("Analog to Digital Morse Signal (700 Hz Detection)")
plt.legend()
plt.grid()
plt.show()
