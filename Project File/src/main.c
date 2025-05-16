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
    int morseBuffer[10];  // adjustable
    int morseIndex = 0;
    int symbol_space_threshold;
    int char_space_threshold;
    char decoded;
    int i;



void lcd_scroll_sample(int value) {
    static int lcd_pos = 0;
    char c = value ? '1' : '0';

    // Move cursor to the current position
    lcd_set_cursor(0, lcd_pos);
    lcd_put_char(c);

    // Advance position, wrap around if at end of line (assuming 16x2 LCD)
    lcd_pos++;
    if (lcd_pos >= 16) {
        lcd_pos = 0;
        lcd_set_cursor(0, 0);
        // Optionally clear the row or overwrite
    }
}


void sample()
{
    if (comparator_read())
    {
        leds_set(1, 1, 0);
        sample_ready = 1;   // Set flag to indicate sample is ready
        sampled_value = 1;  // Store the sampled value
        lcd_scroll_sample(sampled_value);  // Scroll the sampled value on LCD
        
    }
    else
    {
        leds_set(0, 0, 1);
        sample_ready = 1;   // Set flag to indicate sample is ready
        sampled_value = 0;  // Store the sampled value
        lcd_scroll_sample(sampled_value);  // Scroll the sampled value on LCD
    }
}


int main(void)
{
    // Variable declarations moved to the top
    

    leds_init();                 // Initialize LEDs
    timer_init(100000);          // Initialize timer
    comparator_init();           // Initialize comparator
    timer_set_callback(sample);  // Set callback function for timer
    timer_enable();              // Start timer
    leds_set(0, 0, 0);           // Turn off all LEDs
    lcd_init();                  // Initialize LCD
    // lcd_clear();          not sure if these are needed
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
                        leds_set(1, 0, 0);  // Turn on red LED
                        morseBuffer[morseIndex++] = 1;
                    }
                    else // Dot
                    {
                        // handle dot
                        leds_set(0, 1, 0);  // Turn on green LED
                        morseBuffer[morseIndex++] = 0;
                    }

                }
                else
                {
                    if (!queue_enqueue(&Average_data.queue0lens, Average_data.count))
                    {
                        queue_dequeue(&Average_data.queue0lens, &Average_data.value);  // Dequeue if full
                    }

                    Average_data.sum0 = 0;
                    Average_data.size0 = queue_size(&Average_data.queue0lens);
                    for (i = 0; i < Average_data.size0; i++)
                    {
                        queue_dequeue(&Average_data.queue0lens, &Average_data.value);
                        Average_data.sum0 += Average_data.value;
                        queue_enqueue(&Average_data.queue0lens, Average_data.value);  // Re-enqueue the value
                    }
                    Average_data.avg0 = Average_data.sum0 / Average_data.size0;  // Calculate the average

                    // Define thresholds for spaces
                    symbol_space_threshold = Average_data.avg0 * 2;  // Example: 2x avg0
                    char_space_threshold = Average_data.avg0 * 5;    // Example: 5x avg0

                    // Classify the space
                    if (Average_data.count < symbol_space_threshold)
                    {
                        // Symbol space
                        leds_set(0, 0, 1);  // Turn on blue LED (example for symbol space)
                        // Handle symbol space, still in same character, so no action needed
                    }
                    else if (Average_data.count < char_space_threshold)
                    {
                        // Character space
                        leds_set(1, 1, 0);  // Turn on yellow LED (example for character space)
                        // Handle character space
                        if (morseIndex > 0) {
                            decoded = decodeMorse(morseRoot, morseBuffer, morseIndex);
                            lcd_put_char(decoded);  // or printf("%c", decoded);
                            morseIndex = 0;
                        }
                    }
                    else
                    {
                        // Word space
                        leds_set(1, 0, 1);  // Turn on purple LED (example for word space)
                        // Handle word space
                        if (morseIndex > 0) {
                            decoded = decodeMorse(morseRoot, morseBuffer, morseIndex);
                            lcd_put_char(decoded);
                            morseIndex = 0;
                        }
                        lcd_put_char(' ');
                    }
                }

                Average_data.previous_value = sampled_value;  // Update previous value
                Average_data.count = 1;                       // Reset count
            }
        }
    }

    return 0;
}
