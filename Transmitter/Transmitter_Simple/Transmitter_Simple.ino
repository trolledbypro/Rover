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
const int CE = 9;                   // Chip Enable
const int CSN = 8;                  // Chip Select NOT

const byte address[6] = "00001";    // Address can be any 5 bit byte array
                                    // Must match receiver

// Create Radio Object
RF24 radio(9, 8);

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
}

// Arduino loop section
void loop() {
    const char text[] = "Hello World!";
    radio.write(&text, sizeof(text));   // Pass data by reference, also must pass size
    delay(1000);                    // 1 second delay
}
