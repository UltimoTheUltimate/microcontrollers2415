#include <platform.h>
#include "leds.h"
#include "timer.h"
#include "comparator.h"
#include "lcd.h"
#include "squeue.h"
#include "MorseTree.h"

#define QUEUE_SIZE 10  // Define the size of the queues

struct average_data
{
    int avg1;
    int avg0;
    int count;
    int previous_value;
    int sum1;   // Sum for queue1lens
    int sum0;   // Sum for queue0lens
    int value;  // Temporary value for dequeuing
    int size1;  // Size of queue1lens
    int size0;  // Size of queue0lens
    Queue queue0lens;
    Queue queue1lens;
} Average_data;

volatile int sample_ready = 0;   // Flag to indicate if a sample is ready
volatile int sampled_value = 0;  // Variable to store the sampled value


Node* morseRoot;
int morseBuffer[10];  // Morse buffer, adjustable
int morseIndex = 0;
char decodedMessage[100] = {0};  // string buffer, adjustable
int messageIndex = 0;
int symbol_space_threshold;
int char_space_threshold;
char decoded;
int i;



void sample()
{
    if (comparator_read())
    {
        leds_set(1, 1, 0);
        sample_ready = 1;   // Set flag to indicate sample is ready
        sampled_value = 1;  // Store the sampled value
        // lcd_scroll_sample(sampled_value);  // Removed LCD scroll
        // Optionally show sampled value on LCD (optional, can be omitted)
    }
    else
    {
        leds_set(0, 0, 1);
        sample_ready = 1;   // Set flag to indicate sample is ready
        sampled_value = 0;  // Store the sampled value
        // lcd_scroll_sample(sampled_value);  // Removed LCD scroll
        // Optionally show sampled value on LCD (optional, can be omitted)
    }
}


int main(void)
{
    // Variable declarations moved to the top


    leds_init();                 // Initialize LEDs
    timer_init(1500000);          // Initialize timer
    comparator_init();           // Initialize comparator
    timer_set_callback(sample);  // Set callback function for timer
    timer_enable();              // Start timer
    leds_set(0, 0, 0);           // Turn off all LEDs
    lcd_init();                  // Initialize LCD
    lcd_clear();          
    // lcd_set_cursor(0, 0);

    // Initialize the queues
    queue_init(&Average_data.queue0lens, QUEUE_SIZE);
    queue_init(&Average_data.queue1lens, QUEUE_SIZE);

    // Initialize Morse tree
    morseRoot = buildMorseTree();

    while (1)
    {
        if (sample_ready)
        {
            sample_ready = 0;  // Reset flag
            if (sampled_value)
            {
                leds_set(1, 0, 0);  // Turn on red LED
            }
            else
            {
                leds_set(0, 1, 0);  // Turn on green LED
            }

            if (sampled_value == Average_data.previous_value)
            {
                Average_data.count++;  // Increment count
            }
            else
            {
                // Store the count in the appropriate queue
                if (sampled_value)
                {
                    if (!queue_enqueue(&Average_data.queue1lens, Average_data.count))
                    {
                        queue_dequeue(&Average_data.queue1lens, &Average_data.value);  // Dequeue if full
                    }

                    Average_data.sum1 = 0;
                    Average_data.size1 = queue_size(&Average_data.queue1lens);
                    for (i = 0; i < Average_data.size1; i++)
                    {
                        queue_dequeue(&Average_data.queue1lens, &Average_data.value);
                        Average_data.sum1 += Average_data.value;
                        queue_enqueue(&Average_data.queue1lens, Average_data.value);  // Re-enqueue the value
                    }
                    Average_data.avg1 = Average_data.sum1 / Average_data.size1;  // Calculate the average

                    if(Average_data.count > Average_data.avg1)  // Dash
                    {
                        // handle dash
                        morseBuffer[morseIndex++] = 1;
                    }
                    else // Dot
                    {
                        // handle dot
                        morseBuffer[morseIndex++] = 0;
                    }

                }
                else
                {
                    if (!queue_enqueue(&Average_data.queue0lens, Average_data.count))
                    {
                        queue_dequeue(&Average_data.queue0lens, &Average_data.value);  // Dequeue if full
                    }

        
                    // Define thresholds for spaces

                    // Classify the space
                    if (Average_data.count < Average_data.avg1)
                    {
                        // Symbol space
                        // Handle symbol space, still in same character, so no action needed
                    }
                    else if(Average_data.count > (Average_data.avg1*3) )
                    {
                        // Word space
                        // Handle word space
                        if (morseIndex > 0) {
                            decoded = decodeMorse(morseRoot, morseBuffer, morseIndex);
                            morseIndex = 0;
                            if (messageIndex < sizeof(decodedMessage) - 1) {
                                decodedMessage[messageIndex++] = decoded;
                            }
                        }
                        if (messageIndex < sizeof(decodedMessage) - 1) {
                            decodedMessage[messageIndex++] = ' ';  // add space string to buffer for new word
                        }
                        decodedMessage[messageIndex] = '\0';
                        lcd_set_cursor(0, 0);
                        lcd_print(decodedMessage);
                    }
                    else if (Average_data.count > Average_data.avg1 )
                    {
                        // Character space
                        // Handle character space
                        if (morseIndex > 0) {
                            decoded = decodeMorse(morseRoot, morseBuffer, morseIndex);  // tree walk
                            morseIndex = 0;  // reset to decode new character
                            if (messageIndex < sizeof(decodedMessage) - 1) {
                                decodedMessage[messageIndex++] = decoded;
                                decodedMessage[messageIndex] = '\0';  // null terminate string
                                lcd_set_cursor(0, 0);
                                lcd_print(decodedMessage);
                            }
                        }
                    }
                    
                }

                Average_data.previous_value = sampled_value;  // Update previous value
                Average_data.count = 1;                       // Reset count
            }
        }
    }

    return 0;
}
