// Transmitter Simple String Testing Code
// By: Zachary Zalass 40098539
// References: https://lastminuteengineers.com/nrf24l01-arduino-wireless-communication/
//             https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
//             Arduino Class Documentation (https://nrf24.github.io/RF24/)

// Include required libraries
#include <SPI.h>                    // Controls SPI
#include <nRF24L01.h>               // Controls Radio
#include <RF24.h>                   // Controls Radio

// Define constants
#define CE 9                        // Chip Enable
#define CSN 8                       // Chip Select NOT

#define enableButton 20             // Button input pin
#define directionButton 21          // Direction button input pin
#define X_pin 14                    // Joystick x-axis
#define Y_pin 15                    // Joystick y-axis

struct packet {
    byte xValue = 0;                // Store potentiometer value
    byte yValue = 0;                // Store potentiometer value
    byte enableState= 0;            // Store button state for enable
    byte Direction = 0;         // Store button state for direction change
    // Byte due to PWM of sample DC motor
};

const byte address[6] = "00001";    // Address can be any 5 bit byte array
                                    // Must match receiver

// Create Radio Object
RF24 radio(CE, CSN);

// Create Packet
packet payload;

// Arduino Setup Section
void setup() {
    radio.begin();                  // Activate Radio Object, uses default Arduino SPI bus
    Serial.begin(9600);
    if (radio.begin()) {
        Serial.println("Radio is alive");
    }

    // Code section below from Arduino Class Documentation
    if (!radio.begin()) {
        Serial.println("radio hardware not responding!");
        while (1) {}                //hold program in infinite loop to prevent subsequent errors
    }
    // End section from Arduino Class Documentation

    radio.openWritingPipe(address); // Create pipe to radio to write data and sets address
                                    // Defaults on pipe 0 (can have up to six pipes per receiver)
    radio.stopListening();          // Shuts off receiver function, this transceiver will only tramsit from now on
                                    // Put the above line in the loop section if we want to receive an acknowledgement packet 

    pinMode(enableButton, INPUT);            // Activate button input pin
    pinMode(directionButton, INPUT);         // Activate button input pin

}

// Arduino loop section
void loop() {
    payload.xValue = map(analogRead(X_pin), 0, 1023, 0, 255);// Record value of potentiometer (0-1024 mapped to byte size, 0-255)
    payload.yValue = map(analogRead(Y_pin), 0, 1023, 0, 255);// Record value of potentiometer (0-1024 mapped to byte size, 0-255)
    payload.enableState = digitalRead(enableButton);         // Record button state
    if (payload.enableState == 1)
    {
      Serial.println("Enable ON");
    }
    else if (payload.enableState == 0)
    {
      Serial.println("Enable OFF");
    }
    Serial.print("X value: ");
    Serial.println(payload.xValue);
    Serial.print("Y value: ");
    Serial.println(payload.yValue);
    payload.Direction = digitalRead(directionButton);    // Record button state

    radio.write(&payload, sizeof(payload));   // Pass data by reference, also must pass size
    delay(10);                                // 1 second delay
}
