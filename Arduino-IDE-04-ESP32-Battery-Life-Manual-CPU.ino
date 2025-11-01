#include <Arduino.h>

#define LED_BUILTIN 8// On ESP32, GPIO 8 is often used for the built-in LED (Can also connect an external LED here)

// Initial CPU frequency in MHz
uint32_t cpu_freq_mhz = 40;//we will toggle between 40 and 160 mhz
// Variable to monitor elapsed time between cpu frequency changes
long previousMillis = 0;

int ledState = LOW; // Initial state of the LED

//helper function to simulate a time-intensive task and measure its duration
long timeIntensiveTask()
{
    previousMillis = millis();
    volatile uint32_t sum = 0;
    for (uint32_t i = 0; i < 60000000; i++) 
    {
        sum += i;
    }

    return millis() - previousMillis;
}

//helper function to blink the LED
void blink(int numBlinks,int blinkDelay)
{
    for(int i=0;i<numBlinks;i++)
    {
         // Blink the LED to indicate that the ESP32 is awake
        digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
        delay(blinkDelay);                      // wait for half a second
        digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
        delay(blinkDelay);                      // wait for half a second
    }
}

void setup() 
{
    Serial.begin(9600);
    //for an ESP32-C3
    //minimum 40 mhz
    //max 160mhz
    //160 mhz default
    setCpuFrequencyMhz(cpu_freq_mhz);//initialize to 40 mhz

    // Initialize the built-in LED pin as an output
    pinMode(LED_BUILTIN, OUTPUT);

    // Initial blink to indicate startup
    blink(3,300);
}

void loop() 
{
    //Validate current CPU frequency
    uint32_t currentFreq = getCpuFrequencyMhz();

    long time = timeIntensiveTask();
    float timeSecs = (float)time / 1000.0;

    Serial.print("Time intensive task took: ");
    Serial.print(timeSecs);
    Serial.print(" seconds at ");
    Serial.print(currentFreq);
    Serial.println(" mhz");

    // Toggle CPU frequency between 40 MHz and 160 MHz
    if(currentFreq == 40)
    {
        cpu_freq_mhz = 160;
    }
    else
    {
        cpu_freq_mhz = 40;
    }
    setCpuFrequencyMhz(cpu_freq_mhz);

    // Blink the LED to indicate a frequency change
    blink(2,300);
}