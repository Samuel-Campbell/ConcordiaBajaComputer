#include <I2C_Anything.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

unsigned long time = 0;
unsigned long decade = 5000;
boolean first = true;
double angle;


int16_t ax, ay, az; //16 bit integer
int16_t gx, gy, gz; //16 bit integer

float accx, accy, accz;
float gyx, gyy, gyz;
#define OUTPUT_READABLE_ACCELGYRO

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #endif

    accelgyro.initialize(); 

    accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
    
    accelgyro.setXAccelOffset(-713);
    accelgyro.setYAccelOffset(-1389);
    accelgyro.setZAccelOffset(845);
    accelgyro.setXGyroOffset(58);
    accelgyro.setYGyroOffset(-21);
    accelgyro.setZGyroOffset(32);
    Wire.begin(3);

    Serial.begin(9600);
    while(millis() < 5000){
    }  
}

void loop() {
  
     time = millis();
   if (first && (time - decade >= 200) && time > decade){
     accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); 
     accx = ax/16383.5; 
     accy = ay/16383.5;
     accz = az/16383.5;
     gyx = gx/16.3835;
     gyy = gy/16.3835;
     gyz = gz/16.3835;
     angle = analogRead(A3)* (5.0 / 1024); 
     

        
     Wire.beginTransmission(2);
     I2C_writeAnything(accx);
     I2C_writeAnything(accy);
     I2C_writeAnything(accz);
     I2C_writeAnything(gyx);
     I2C_writeAnything(gyy);
     I2C_writeAnything(gyz);
     I2C_writeAnything(angle);
     Wire.endTransmission();
     first = false;
    }
    else if (first == false && (time - decade >= 700) && time > decade){

     accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); 
     accx = ax/16383.5; 
     accy = ay/16383.5;
     accz = az/16383.5;
     gyx = gx/16.3835;
     gyy = gy/16.3835;
     gyz = gz/16.3835;
     angle = analogRead(A3)* (5.0 / 1024); 
      
     Wire.beginTransmission(2);
     I2C_writeAnything(accx);
     I2C_writeAnything(accy);
     I2C_writeAnything(accz);
     I2C_writeAnything(gyx);
     I2C_writeAnything(gyy);
     I2C_writeAnything(gyz);
     I2C_writeAnything(angle);
     Wire.endTransmission();
     first = true;
     decade +=1000;
    }
}



