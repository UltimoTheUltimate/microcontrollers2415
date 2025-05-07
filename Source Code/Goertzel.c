#include <stdio.h>
#include <math.h>

// Constants
#define NULL_PTR ((void*)0)  // NULL replacement
#define PI 3.14159265358979323846
#define FRAME_SIZE 128
#define SAMPLE_RATE 8000
#define TARGET_FREQ 700.0f
#define POWER_THRESHOLD 1000.0f  // adjust based on actual tone strength

// Approximate cos(x (in radians)) using Taylor series to remove math.h
float cos_approx(float x)
{
    float x2 = x * x;
    float n0 = 1.0f;
    float n1 = x2 / 2.0f;
    float n2 = (x2 * x2) / 24.0f;
    float n3 = (x2 * x2 * x2) / 720.0f;
    float n4 = (x2 * x2 * x2 * x2) / 40320.0f;
    return n0 - n1 + n2 - n3 + n4;
}

// Compute Goertzel power for target frequency
float goertzel_algorithm(float signals[], int number_of_samples, float wanted_frequency, float sampleRate)
{
    int k = (int)(0.5 + ((number_of_samples * wanted_frequency) / sampleRate));
    float omega = (2.0 * PI * k) / number_of_samples;
    float coeff = 2.0 * cos_approx(omega);
    float s0 = 0.0f;
    float s1 = 0.0f;
    float s2 = 0.0f;

    for (int i = 0; i < number_of_samples; i++)
    {
        s0 = signals[i] + coeff * s1 - s2;
        s2 = s1;
        s1 = s0;
    }

    // Calculate power (avoids sqrt)
    float power = s1 * s1 + s2 * s2 - coeff * s1 * s2;

    return power;
}

void tone_to_bits(float* signal, int signal_length, char* bit_stream, int* num_bits)
{
    int total_frames = signal_length / FRAME_SIZE;
    *num_bits = 0;

    for (int i = 0; i < total_frames; i++)
    {
        if (i * FRAME_SIZE + FRAME_SIZE > signal_length) {
            break;  // Prevent going out of bounds
        }

        float* frame_start = &signal[i * FRAME_SIZE];
        float power = goertzel_algorithm(frame_start, FRAME_SIZE, TARGET_FREQ, SAMPLE_RATE);

        // Simple thresholding to detect tone presence
        if (power > POWER_THRESHOLD)
            bit_stream[*num_bits] = '1';
        else
            bit_stream[*num_bits] = '0';

        (*num_bits)++;
    }

    bit_stream[*num_bits] = '\0';  // Null-terminate as a string if needed
}

/*
int main()
{
    // Generate a 1-second signal for testing
    int signal_length = SAMPLE_RATE; // 1 second of audio at 8000 Hz
    float signal[signal_length];
    generate_test_signal(signal, signal_length, TARGET_FREQ, SAMPLE_RATE);

    // Decode the signal into bits
    char bitstream[1000];
    int bit_count = 0;
    tone_to_bits(signal, signal_length, bitstream, &bit_count);

    // Print the bitstream (this should correspond to the SOS pattern)
    printf("Decoded bitstream: %s\n", bitstream);

    return 0;
}
 */
