#include <Wire.h>
#include <I2C_Anything.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

//LIQUID CRYSTAL DISPLAY
#define I2C_ADDR    0x3F  // Define I2C Address where the PCF8574A is
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

unsigned long time = 0;
unsigned long decade = 5000;
float velocity = 0;
float accx, accy, accz;
float gyx, gyy, gyz;
float angle;
double ambientTemp, beltTemp;
int rpm = 0;
String data = "";
String acquisition = "logger";
boolean first= true;

/* SD CARD */
const String header = "Speed, RPM, beltTemp, ambientTemp, Accel x, Accel y, Accel z, Gyro x, Gyro y, Gyro z, Angle"; //Header fir cvs file
int testCount = 0;
File logFile;

void setup() {

  testCount = EEPROM.read(1);
  if (testCount >= 100)
  {
    testCount = 0;
  }

  testCount++;
  EEPROM.write(1, testCount);
  
  lcd.begin(16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE); // Switch on the backlight
  lcd.setBacklight(HIGH);
  
 
  lcd.setCursor(0,0);
  lcd.print("initializing SD CARD");
  delay(300);
  
  pinMode (10, OUTPUT);
  //SD CARD
  //Check if SD card is present
  if (!SD.begin(2))
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("initialization failed!");
    delay(1000);
  }
  else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("success");
    delay(300);
  }

  acquisition += String(testCount) + ".csv";

  char fileNameCharArray[acquisition.length()];
  acquisition.toCharArray(fileNameCharArray, acquisition.length() + 1);

  logFile = SD.open(fileNameCharArray, O_WRITE | O_CREAT | O_APPEND);
  if (logFile)
  {
    logFile.println();
    logFile.print("---, ---, Test ");
    logFile.print(String(testCount));
    logFile.println(", ---, ---");
    logFile.println(header);
    logFile.close();
  }
  
  lcd.clear();
  //Vehicule stats
  lcd.setCursor(0,0);
  lcd.print("Speed:");
  lcd.setCursor(0, 1);
  lcd.print("RPM:");
  lcd.setCursor(13,0);
  lcd.print(testCount);
  
  Wire.begin(2);
  Wire.onReceive(event);
  while (millis() < 5000) {
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  time = millis();
  if (first && (time - decade >= 500) && time > decade) {
    char fileNameCharArray[acquisition.length()];
    acquisition.toCharArray(fileNameCharArray, acquisition.length() + 1);
    logFile = SD.open(fileNameCharArray, O_WRITE | O_APPEND);
    if (logFile)
    {
      data  = String(velocity) + ", " + String(rpm) +", " + String(beltTemp) + ", " + String(ambientTemp) + ", " + String(accx) + ", " + String(accy) + ", " + String(accz) + ", " + String(gyx) + ", " + String(gyy) + ", " + String(gyz) + ", " + String(angle);
      logFile.println(data);
      logFile.close();
    }
    lcd.setCursor(6, 0);
    lcd.print(velocity);
    lcd.setCursor(4,1);
    lcd.print(rpm);
    lcd.setCursor(11,1);
    lcd.print(beltTemp);
    first = false ;
  }

    else if (first == false && (time - decade >= 1000) && time > decade) {
      char fileNameCharArray[acquisition.length()];
      acquisition.toCharArray(fileNameCharArray, acquisition.length() + 1);
      logFile = SD.open(fileNameCharArray, O_WRITE | O_APPEND);
      if (logFile)
      {
        data  = String(velocity) + ", " + String(rpm) +", " + String(beltTemp) + ", " + String(ambientTemp) + ", " + String(accx) + ", " + String(accy) + ", " + String(accz) + ", " + String(gyx) + ", " + String(gyy) + ", " + String(gyz) + ", " + String(angle);
        logFile.println(data);
        logFile.close();
      }
      data  = String(velocity) + ", " + String(accx) + ", " + String(accy) + ", " + String(accz) + ", " + String(gyx) + ", " + String(gyy) + ", " + String(gyz) + ", " + String(angle);
      lcd.setCursor(6, 0);
      lcd.print(velocity);
      lcd.setCursor(4,1);
      lcd.print(rpm);
      lcd.setCursor(11,1);
      lcd.print(beltTemp);
      first = true;
      decade += 1000;
    }
}
  

    // called by interrupt service routine when incoming data arrives
    void event (int howMany)
    {
      if (howMany == (sizeof accx) + (sizeof accy) + (sizeof accz) + (sizeof gyx) + (sizeof gyy) + (sizeof gyz) + (sizeof angle)) {
        I2C_readAnything (accx);
        I2C_readAnything (accy);
        I2C_readAnything (accz);
        I2C_readAnything (gyx);
        I2C_readAnything (gyy);
        I2C_readAnything (gyz);
        I2C_readAnything (angle);
      }
      else if (howMany == (sizeof velocity)) {
        I2C_readAnything(velocity);
      }
      else if (howMany == (sizeof rpm)){
        I2C_readAnything(rpm);
      }
      else if (howMany == (sizeof ambientTemp) + (sizeof beltTemp)){
        I2C_readAnything(ambientTemp);
        I2C_readAnything(beltTemp);
      }
    }  // end of receiveEvent
