// Receiver Simple String Testing Code
// By: Zachary Zalass 40098539
// References: https://lastminuteengineers.com/nrf24l01-arduino-wireless-communication/
//             https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
//             Arduino Class Documentation (https://nrf24.github.io/RF24/)

// Include required libraries
#include <SPI.h>                    // Controls SPI
#include <nRF24L01.h>               // Controls Radio
#include <RF24.h>                   // Controls Radio
#include <Servo.h>                  // Servo control

// Define constants
#define CE 7                        // Chip Enable
#define CSN 8                       // Chip Select NOT

#define ENABLE 5                    // ESC enable
#define DIRA 3                      // Direction pin #1
#define DIRB 4                      // Direciton pin #2, inverse of DIRA
#define SERVOPIN 9                  // Pin for servo output

const byte address[6] = "00001";    // Address can be any 5 bit byte array
                                    // Must match transmitter

struct packet {
    byte xValue = 0;                // Store potentiometer value
    byte yValue = 0;                // Store potentiometer value
    byte enableState= 0;            // Store button state for enable
    byte Direction = 0;             // Store button state for direction change
    // Byte due to PWM of sample DC motor
};

byte lastEnableState = LOW;         // Integer to represent previous state of enable
byte lastDirectionState = HIGH;     // Integer to represent previous direction state
                                    // 1 = FORWARD (default)
                                    // 0 = BACKWARDS

// Create Radio Object
RF24 radio(CE , CSN);

// Create servo object
Servo servo;

// Create Packet
packet payload;

// Arduino Setup Section
void setup() {
    pinMode(ENABLE,OUTPUT);
    pinMode(DIRA,OUTPUT);
    pinMode(DIRB,OUTPUT);
    servo.attach(SERVOPIN);
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
        Serial.print("X value: ");
        Serial.print(payload.xValue);
        Serial.print("\tY value: ");
        Serial.print(payload.yValue);
        Serial.print("\tEnable State: ");
        Serial.print(payload.enableState);
        Serial.print("\tDirection state: ");
        Serial.println(payload.Direction);   
    }

    int currentEnableState = payload.enableState;
    int currentDirectionState = payload.Direction;
    int rawDirection = payload.xValue;
    int direction = map(rawDirection, 0, 255, 0, 180);

    servo.write(direction);

    //Debug below
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);

    if (currentEnableState == 1) {
        digitalWrite(ENABLE, HIGH);
        //Serial.println("Enabled ON");
    }

    else if (currentEnableState == 0) {
        digitalWrite(ENABLE, LOW);
       // Serial.println("Enabled OFF");
    }
    
    if (currentDirectionState == 1) {
        digitalWrite(DIRA, HIGH);
        digitalWrite(DIRB, LOW);
        //Serial.println("Direction Forward");
    }

    else if (currentDirectionState == 0) {
        digitalWrite(DIRA, LOW);
        digitalWrite(DIRB, HIGH);
        //Serial.println("Direction Backwards");
    }
    /*
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
    */
}
