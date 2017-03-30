/*************************************************** 
  This is a library example for the MLX90614 Temp Sensor

  Designed specifically to work with the MLX90614 sensors in the
  adafruit shop
  ----> https://www.adafruit.com/products/1748
  ----> https://www.adafruit.com/products/1749

  These sensors use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include <I2C_Anything.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

unsigned long time = 0;
unsigned long decade = 5000;
boolean first = true;
double ambientTemp;
double beltTemp;

void setup() {
  Serial.begin(9600);

  Serial.println("Adafruit MLX90614 test");  

  mlx.begin();  
  
  Wire.begin(4);
  
  Serial.begin(9600);
    while(millis() < 5000){
    } 
}

void loop() {
  
  time = millis();
  
  if (first && (time - decade >= 400) && time > decade){
    ambientTemp = mlx.readAmbientTempC(); 
    beltTemp = mlx.readObjectTempC(); 
    
    Wire.beginTransmission(2);
    I2C_writeAnything(ambientTemp);
    I2C_writeAnything(beltTemp);
    Wire.endTransmission();
    first = false;
  }
  else if (first == false && (time - decade >= 900) && time > decade){
    ambientTemp = mlx.readAmbientTempC(); 
    beltTemp = mlx.readObjectTempC(); 
    Wire.beginTransmission(2);
    I2C_writeAnything(ambientTemp);
    I2C_writeAnything(beltTemp);
    Wire.endTransmission();
    first = true;
    decade +=1000;
  }
}
