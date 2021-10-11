//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

void setup()
{
  while (!Serial);
    Serial.begin(9600);
    // initialize the transceiver on the SPI bus
   
  // print example's introductory prompt
  Serial.println(F("RF24/examples/GettingStarted"));
  radio.begin();
  
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {} // hold in infinite loop
  }
    
  //set the address
  radio.openReadingPipe(0, address);
  
  //Set module as receiver
  radio.startListening();
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}
