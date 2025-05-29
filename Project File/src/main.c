#include <platform.h>
#include "leds.h"
#include "timer.h"
#include "comparator.h"
#include "lcd.h"
#include "squeue.h"
#include "MorseTree.h"

#define QUEUE_SIZE 20  // less than 20 may increase chance of errors due to average drift
#define SAMPLE_QUEUE_SIZE 32

int avg1;
int count;
int previous_value;
int sum1;   // Sum for queue1lens
int value;  // Temporary value for dequeuing
int size1;  // Size of queue1lens
Queue queue1lens;

volatile int sample_ready = 0;   // Flag to indicate if a sample is ready
volatile int sampled_value = 0;  // Variable to store the sampled value

Node* morseRoot;
int morseBuffer[10];  // Morse buffer, adjustable
int morseIndex = 0;
char decoded;
int i;

int sampleQueue[SAMPLE_QUEUE_SIZE];
int sampleQueueHead = 0;
int sampleQueueTail = 0;

// Enqueue function for sample queue
int enqueue_sample(int value) {
    int nextHead = (sampleQueueHead + 1) % SAMPLE_QUEUE_SIZE;
    if (nextHead == sampleQueueTail) {
        // Queue full, drop sample or handle overflow
        return 0;
    }
    sampleQueue[sampleQueueHead] = value;
    sampleQueueHead = nextHead;
    return 1;
}

// Dequeue function for sample queue
int dequeue_sample(int *value) {
    if (sampleQueueHead == sampleQueueTail) {
        // Queue empty
        return 0;
    }
    *value = sampleQueue[sampleQueueTail];
    sampleQueueTail = (sampleQueueTail + 1) % SAMPLE_QUEUE_SIZE;
    return 1;
}

void sample()
{
    int value = comparator_read() ? 1 : 0;
    enqueue_sample(value);
}

int main(void)
{
    


    leds_init();                 // Initialize LEDs
    timer_init(250000);          // Initialize timer
    comparator_init();           // Initialize comparator
    timer_set_callback(sample);  // Set callback function for timer
    timer_enable();              // Start timer
    leds_set(0, 0, 0);           // Turn off all LEDs
    lcd_init();                  // Initialize LCD
    lcd_clear();          
    // lcd_set_cursor(0, 0);

   
    queue_init(&queue1lens, QUEUE_SIZE);

    // Initialize Morse tree
    morseRoot = buildMorseTree();

    while (1)
    {
        int has_sample = 0;
        int current_sample = 0;
        has_sample = dequeue_sample(&current_sample);
        if (has_sample)
        {
            if (current_sample) {
                leds_set(1, 0, 0); // Green for 1 detected
            } else {
                leds_set(0, 1, 0); // Red for 0 detected
            }// I have been bashing my head on this for half an hour here... Why do the conditions for this have to be reversed for red and green to be shown at 1 or 0??? 
            sampled_value = current_sample;
            if (sampled_value == previous_value)
            {
                count++;  
            }
            else
            {
                
                if (sampled_value)
                {
                    if (!queue_enqueue(&queue1lens, count))
                    {
                        queue_dequeue(&queue1lens, &value);  // Dequeue if full
                    }

                    sum1 = 0;
                    size1 = queue_size(&queue1lens);
                    for (i = 0; i < size1; i++)
                    {
                        queue_dequeue(&queue1lens, &value);
                        sum1 += value;
                        queue_enqueue(&queue1lens, value);  // Re-enqueue the value
                    }
                    avg1 = sum1 / size1;  // Calculate the average

                    if(count > avg1)  // Dash
                    {
                        // handle dash
                        if (morseIndex < sizeof(morseBuffer)/sizeof(morseBuffer[0])) {
                            morseBuffer[morseIndex++] = 1;
                        } else {
                            // Optionally reset or handle overflow
                            morseIndex = 0;
                        }
                    }
                    else // Dot
                    {
                        // handle dot
                        if (morseIndex < sizeof(morseBuffer)/sizeof(morseBuffer[0])) {
                            morseBuffer[morseIndex++] = 0;
                        } else {
                            // Optionally reset or handle overflow
                            morseIndex = 0;
                        }
                    }

                }
                else
                {
                    
                    
                    if(count > (avg1*3))
                    {
                        
                            if (morseIndex > 0) {
                                decoded = decodeMorse(morseRoot, morseBuffer, morseIndex);
                                morseIndex = 0;
                                lcd_scroll_char(decoded);
                                lcd_scroll_char(' ');  // Add space after the character
                            }
                        
                    }
                    else if (count > avg1)
                    {
                        
                        // Character space
                        if (morseIndex > 0) {
                            decoded = decodeMorse(morseRoot, morseBuffer, morseIndex);  
                            morseIndex = 0;  
                            lcd_scroll_char(decoded);
                        }
                    }
                    else
                    {
                        
                        
                    }
                }

                previous_value = sampled_value;  // Update previous value
                count = 1;                       // Reset count
            }
        }
        else
        {
            
        }
        //else                 leds_set(1, 0, 0);  // Turn on green LED;
    }

    return 0;
}
