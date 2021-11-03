// Receiver Simple String Testing Code
// By: Zachary Zalass 40098539
// References: https://lastminuteengineers.com/nrf24l01-arduino-wireless-communication/
//             https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
//             Arduino Class Documentation (https://nrf24.github.io/RF24/)

// Include required libraries
#include <SPI.h>                    // Controls SPI
#include <nRF24L01.h>               // Controls Radio
#include <RF24.h>                   // Controls Radio

// Define constants
const int CE = 7;                   // Chip Enable
const int CSN = 8;                  // Chip Select NOT

const int ENABLE = 5;               // ESC enable
const int DIRA = 3;                 // Direction pin #1
const int DIRB = 4;                 // Direciton pin #2, inverse of DIRA

const byte address[6] = "00001";    // Address can be any 5 bit byte array
                                    // Must match transmitter

struct packet {
    byte potValue = 0;              // Store potentiometer value
    byte enableState= 0;            // Store button state for enable
    byte swapDirection = 0;         // Store button state for direction change
    // Byte due to PWM of sample DC motor
};

byte lastEnableState = LOW;         // Integer to represent previous state of enable
byte lastDirectionState = HIGH;     // Integer to represent previous direction state
                                    // 1 = FORWARD (default)
                                    // 0 = BACKWARDS

// Create Radio Object
RF24 radio(CE , CSN);

// Create Packet
packet payload;

// Arduino Setup Section
void setup() {
    radio.begin();                      // Activate Radio Object, uses default Arduino SPI bus
    Serial.begin(9600);
    if (radio.begin()) {
        Serial.println(F("Radio is alive"));
    }

    // Code section below from Arduino Class Documentation
    if (!radio.begin()) {
        Serial.println(F("radio hardware not responding!"));
        while (1) {}                    //hold program in infinite loop to prevent subsequent errors
    }
    // End section from Arduino Class Documentation

    radio.openReadingPipe(0, address);  // Create pipe to radio to read data and sets address
                                        // Defaults on pipe 0 (can have up to six pipes per receiver)
    radio.startListening();             // Turns on receiver function, this transceiver will only receive from now on
                                        // Put the above line in the loop section if we want to send an acknowledgement packet 
    
}

// Arduino loop section
void loop() {
    if (radio.available()) {                    // If data is available in buffer
        radio.read(&payload, sizeof(payload));  // Copy received packet to memory  
        // Print to serial monitor
        Serial.print("Pot Value: ").print(payload.potValue).print("Enable State: ").print(payload.enableState).print("Direction change state: ").println(payload.swapDirection);   
    }

    int currentEnableState = payload.enableState;
    int currentDirectionState = payload.swapDirection;

    if (currentEnableState != lastEnableState) {
        if (currentEnableState == HIGH) 
            digitalWrite(ENABLE, HIGH);

        else if (currentEnableState == LOW)
            digitalWrite(ENABLE, LOW);
    }


    if (currentDirectionState != lastDirectionState) {
        if (lastDirectionState == HIGH) {
            digitalWrite(DIRA, HIGH);
            digitalWrite(DIRB, LOW);
        }

        else if (lastDirectionState == LOW) {
            digitalWrite(DIRA, LOW);
            digitalWrite(DIRB, HIGH);
        }
    }
}
