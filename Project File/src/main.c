#include <platform.h>
#include "leds.h"
#include "timer.h"
#include "comparator.h"
#include "lcd.h"
#include "squeue.h"
#include "MorseTree.h"

#define QUEUE_SIZE 20  // less than 20 may increase chance of errors due to average drift
#define SAMPLE_QUEUE_SIZE 32

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
char lastDisplayedMessage[100] = {0};
int messageIndex = 0;
int symbol_space_threshold;
int char_space_threshold;
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

// Helper function for string comparison (C89, no strcmp)
int str_equal(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == *b);
}

int main(void)
{
    // Variable declarations moved to the top


    leds_init();                 // Initialize LEDs
    timer_init(300000);          // Initialize timer
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
        int has_sample = 0;
        int current_sample = 0;
        has_sample = dequeue_sample(&current_sample);
        if (has_sample)
        {
            leds_set(1, 1, 0); // Sample ready: yellow (red + green)
            sampled_value = current_sample;
            // Remove sample_ready logic, process every dequeued sample
            // ...existing code for processing a sample...
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
                            lcd_scroll_char(decoded);
                            lcd_scroll_char(' ');  // Add space after the character
                        }
                        // Optionally print a space after a word
                        // lcd_set_cursor(0, 0);
                        // lcd_put_char(' ');
                    }
                    else if (Average_data.count > Average_data.avg1 )
                    {
                        // Character space
                        // Handle character space
                        if (morseIndex > 0) {
                            decoded = decodeMorse(morseRoot, morseBuffer, morseIndex);  // tree walk
                            morseIndex = 0;  // reset to decode new character
                            lcd_scroll_char(decoded);
                        }
                    }
                    
                }

                Average_data.previous_value = sampled_value;  // Update previous value
                Average_data.count = 1;                       // Reset count
            }
        }
        else
        {
            leds_set(0, 0, 0); // Sample not ready: blue
        }
        //else                 leds_set(1, 0, 0);  // Turn on green LED;
    }

    return 0;
}
