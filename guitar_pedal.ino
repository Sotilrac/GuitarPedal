/*
* Author: Carlos Asmat
* Purpose: Switch a relay according to a pot input.
*          Display the period on a serila 7-segment display.
*/

#include <SoftwareSerial.h>

SoftwareSerial sevenSeg(3, 2); // RX, TX
const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to
const int relayPin = A2;  // Analog input pin that the potentiometer is attached to
const int pedalPin = A0;

char buf[12];  // number to be printed to serial LCD
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the LCD

int relayState = LOW;
long previousMillis = 0;
const long refreshDelay = 100;
long lastRefresh = 0;

void setup() {

  pinMode(relayPin, OUTPUT);
  pinMode(pedalPin, INPUT);
   // Open serial communications:
  delay(20);
  sevenSeg.begin(9600);
  Serial.begin(9600);
  
  delay(20);
  
  //Reset Display
  sevenSeg.write((byte)0x76); // reset command
  sevenSeg.write((byte)0x7A); // command byte
  sevenSeg.write((byte)0xFF); // display brightness (highher = brighter)
  sevenSeg.write((byte)0x77); // command byte for decimal points
  sevenSeg.write((byte)0x00); // turn them off
  
  SegDisplay(0);
}

void loop() {
  

  unsigned long currentMillis = millis();
  

  
  // read the analog in value:
  sensorValue = analogRead(analogInPin);            
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  
  if(currentMillis - lastRefresh > refreshDelay)
  {
    lastRefresh = currentMillis;
    SegDisplay(outputValue); 
  }
  
  if(digitalRead(pedalPin) == LOW)
  {
    relayState = LOW;
    delay(10); //debounce pedal signal
  }
  else
  {
    if(currentMillis - previousMillis > outputValue)
    {
      // save the last time the relay was toggled 
      previousMillis = currentMillis;   
    
      // if the relay is off turn it on and vice-versa:
      if (relayState == LOW)
        relayState = HIGH;
      else
        relayState = LOW;
    }
  }
  digitalWrite(relayPin, relayState);
           

  delay(2);

}

void SegDisplay(int i)
{
  sprintf(buf, "%04d", i);
  sevenSeg.write((byte)0x76); // reset command
  sevenSeg.print(buf);
}

