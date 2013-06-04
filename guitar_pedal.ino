/*
  Analog input, analog output, serial output
 
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.
 
 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground
 
 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 
 */

#include <SoftwareSerial.h>

SoftwareSerial sevenSeg(2, 3); // RX, TX
char buf[12];

const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = A2; // Analog output pin that the LED is attached to
const int pedalPin = A0;

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

  int relayState = LOW;
  long previousMillis = 0;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(analogOutPin, OUTPUT);
  pinMode(pedalPin, INPUT);
}

void loop() {
  

  unsigned long currentMillis = millis(); 

  
  // read the analog in value:
  sensorValue = analogRead(analogInPin);            
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);  


 if(digitalRead(pedalPin) == LOW)
 {
   relayState = LOW;
 }
 else
 {
  if(currentMillis - previousMillis > outputValue) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (relayState == LOW)
      relayState = HIGH;
    else
      relayState = LOW;
  }
  
  digitalWrite(analogOutPin, relayState);
 }
         

delay(2);
}



/*
* Author: Carlos Asmat
* Purpose: Count parts that go trough a sensor
* and show it on a serial 7-segment disply.
*/




void setup()
{
 // Open serial communications:
  delay(20);
  sevenSeg.begin(9600);
  //Serial.begin(9600);
  
  delay(20);
  
  //Reset Display
  sevenSeg.write((byte)0x76); // reset command
  sevenSeg.write((byte)0x7A); // command byte
  sevenSeg.write((byte)0x00); // display brightness (lower = brighter)
  sevenSeg.write((byte)0x77); // command byte for decimal points
  sevenSeg.write((byte)0x00); // turn them off

  
  for (int i=0; i < sensorNum; i++)
  {
    baseline[i] = CallibrateSensor(sensorPin[i]);
  }
  

  SegDisplay(0);

}

void loop()
{
  int reading;
  
  //Serial.print("Readings:");
  for(int i=0; i< sensorNum; i++)
  {
    reading = analogRead(sensorPin[i]);
    //Serial.print(reading, DEC);
    //Serial.print("\t");
    if (reading-threshold[i] > baseline[i])
    {
      SegDisplay(++count);
      delay(countDelay);
    }
  }
  //Serial.print("\n");
  
  if(digitalRead(buttonPin) == LOW)
  {
    count = 0;
    SegDisplay(count);
  }
  
}

void SegDisplay(int i)
{
  sprintf(buf, "%04d", i);
  sevenSeg.write((byte)0x76); // reset command
  sevenSeg.print(buf);
}

int CallibrateSensor(int a)
{
  long n = 64; //number of readings;
  const int t = 20; //delay btween readings
  int sum = 0;
  
  while(n>0)
  {
    sum += analogRead(a);
    n--;
    delay(t);
  }
  return (int)(sum >> 6);
}

