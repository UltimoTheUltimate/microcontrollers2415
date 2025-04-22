#include <math.h>
#include <stdio.h>
#define PI 3.14159265358979323846

float goertzel_algorithm(float signals[], int number_of_samples, float wanted_frequency,float sampleRate)// in c when an array is passed as argument it changes into a pointer and we cant really measure its length, thats why we need "number_of_samples"
{
int k = (0.5 + ((number_of_samples* wanted_frequency)/ sampleRate));
float omega = (2 * PI * k) / number_of_samples;
float coeff = 2.0 * cos(omega);
float s0 =0;
float s1 =0;
float s2 =0;

for (int i = 0; i < number_of_samples; i++)
{
   s0 = (coeff * s1) - s2 + signals[i];
   s2= s1;
   s1 =s0;
}


float magnitude = sqrt((s1 * s1) + (s2 * s2) - (coeff * s1 *s2));
return magnitude;
}