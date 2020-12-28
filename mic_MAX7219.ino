const int MIC_IN = 34;
int dB; //the variable that will hold the value read from the microphone each time
uint8_t current;

// Including the required Arduino libraries
#include "MD_MAX72xx.h"

// Initialization
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW
#define MAX_DEVICES 15 //this example is using only 6
#define CS_PIN 4
#define DATA_PIN 5
#define CLK_PIN 18
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

int smooth(){
  int i;
  int value = 0;
  int numReadings = 50;
  int temppp;

  for (i = 0; i < numReadings; i++){
    // Read light sensor data.
    temppp=analogRead(MIC_IN);
    if (temppp!=8)
    value = value + temppp;
    else
    i--;
    // 1ms pause adds more stability between reads.
    delay(1);
  }

  // Take an average of all the readings.
  value = value / numReadings;

  // Scale to 8 bits (0 - 255).
  value = value / 4;

  return value;
}

void setup() {
   // Set the baud rate at 9600
  Serial.begin(9600);
  // Intialize the object
  mx.begin();
  // Set the intensity (brightness) of the display (0-15)
  mx.control(MD_MAX72XX::INTENSITY, 7);
  // Clear the display
  mx.clear();
//  pinMode(MIC_IN, INPUT);
}

void loop() {
//  dB=Wire.read();
//  dB=analogRead(MIC_IN);
  dB= smooth(); //Read the ADC value from amplifer 
//  if (dB<0)
//  dB=abs(dB)+50;
//  if (dB>255)
//  dB=254;
  Serial.println (dB);
  dB = map(dB, 0, 1023, 0, 7);
  Serial.println (dB);
  Serial.println(" ");
  Serial.println(" ");

  switch(dB) {
    case 0:
    current = 128;
    break;
    case 1:
    current = 192;
    break;
    case 2:
    current = 224;
    break;
    case 3:
    current = 240;
    break;
    case 4:
    current = 248;
    break;
    case 5:
    current = 252;
    break;
    case 6:
    current = 254;
    break;
    case 7:
    current = 255;
    break;
    default:
    current = 255;
  }
  
  for (int i=mx.getColumnCount()-1; i>=1; i--)
  mx.setColumn(i, mx.getColumn(i-1)); //shift by 1

  mx.setColumn(0, current);
//  delay(100);
}
