#include <platform.h>
#include "tone.h"

// Comment to play with interrupts, uncomment to play with blocking.
//#define BUSY_WAIT
volatile int sample_ready = 0; // Flag to indicate if a sample is ready
volatile int sampled_value = 0; // Variable to store the sampled value



int main(void) {
	leds_init(); // Initialize LEDs
	timer_init(100000); // Initialize timer
	timer_set_callback(sample); // Set callback function for timer
	timer_start(); // Start timer
	
	
	
	while(1) {
		
		if (sample_ready) {
			sample_ready = 0; // Reset flag
			



			


		}
	
	
	
	}
	

// *******************************ARM University Program Copyright � ARM Ltd 2014*************************************   

}
void sample(){

	if (comparator_read()) {
		leds_set(1, 1, 0);
		 sample_ready = 1; // Set flag to indicate sample is ready
		sampled_value = 1; // Store the sampled value
	} else {
		
		
		leds_set(0, 0, 1);
		sample_ready = 1; // Set flag to indicate sample is ready
		sampled_value = 0; // Store the sampled value
		return 0
	}

	
}