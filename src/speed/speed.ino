#include <Wire.h>
#include <I2C_Anything.h>

float counter = 0;
float velocity = 0;
unsigned long time = 0;
unsigned long decade = 5000;
const float tireCircumference = 3.141 * 22 * 0.0254; // in meters. 0.5842 is the tire diameter
boolean first = true;

boolean rising = true;
int hallState;

void setup() {
  // put your setup code here, to run once:
  Wire.begin(1);
  pinMode(2, INPUT);
  while(millis() < 5000){
  }  
  Serial.begin(9600);
}

void loop() {
  time = millis();
  hallState = digitalRead(2);
  
  if(hallState == 1 && rising){
    counter++;
    rising = false;
  }
  else if (hallState ==0 && rising == false){
    rising = true;
  }
  
  if((first == true) && ((time - decade) >= 100) && time > decade){ 
    velocity = (((counter * 3.6) / 30) * tireCircumference / (500)); 
    Wire.beginTransmission(2);
    I2C_writeAnything(velocity);
    Wire.endTransmission();
    counter = 0;
    first = false ;
  }
    else if (((time - decade) >= 600) && time > decade){
    velocity = (((counter * 3.6) / 30) * tireCircumference / (500)); 
    Wire.beginTransmission(2);
    I2C_writeAnything(velocity);
    Wire.endTransmission();
    counter = 0;
    first = true;
    decade += 1000;
  }
}



