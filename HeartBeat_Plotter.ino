/*
  Heart beat plotting!
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 20th, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  Shows the user's heart beat on Arduino's serial plotter

  Instructions:
  1) Load code onto Redboard
  2) Attach sensor to your finger with a rubber band (see below)
  3) Open Tools->'Serial Plotter'
  4) Make sure the drop down is set to 115200 baud
  5) Checkout the blips!
  6) Feel the pulse on your neck and watch it mimic the blips

  It is best to attach the sensor to your finger using a rubber band or other tightening
  device. Humans are generally bad at applying constant pressure to a thing. When you
  press your finger against the sensor it varies enough to cause the blood in your
  finger to flow differently which causes the sensor readings to go wonky.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/

#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;
MAX30105 particleSensor2;

/*
#define SDA_1 25
#define SCL_1 26

#define SDA_2 27
#define SCL_2 14
*/


#define SDA_1 5
#define SCL_1 18

#define SDA_2 21
#define SCL_2 22

#define I2C_FREQ 400000


TwoWire I2C_1 = TwoWire(0);
TwoWire I2C_2 = TwoWire(1);

void setup()
{



  I2C_1.begin(SDA_1, SCL_1, I2C_FREQ);
  I2C_2.begin(SDA_2, SCL_2, I2C_FREQ);
  

  // Initialize sensor
  if (!particleSensor.begin( I2C_1, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 #1 was not found. Please check wiring/power. ");
    while (1);
  }
  
  // Initialize sensor
  if (!particleSensor2.begin( I2C_2, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 #2 was not found. Please check wiring/power. ");
    while (1);
  }
  
  
  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 1600; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  particleSensor2.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

  //Arduino plotter auto-scales annoyingly. To get around this, pre-populate
  //the plotter with 500 of an average reading from the sensor
/*
  //Take an average of IR readings at power up
  const byte avgAmount = 64;
  long baseValue = 0;
  for (byte x = 0 ; x < avgAmount ; x++)
  {
    baseValue += particleSensor.getIR(); //Read the IR value
  }
  baseValue /= avgAmount;

  //Pre-populate the plotter so that the Y scale is close to IR values
  for (int x = 0 ; x < 500 ; x++)
    Serial.printf("%d %d\n",baseValue, baseValue);
   */

  Serial.begin(115200);
  //Serial.println("Initializing...");
}

void loop()
{
    float rawIR  = particleSensor.getFilteredIR();
    float rawIR2 = particleSensor2.getFilteredIR();
    bool hit = particleSensor.detectPulse(rawIR,600);
    bool hit2 = particleSensor2.detectPulse(rawIR2,2000);
    if (hit) {
       Serial.printf("hit 1\n");
    }
    if (hit2) {
       Serial.printf("hit 2\n");
    }
    Serial.printf("%f %f %f %f\n", rawIR, rawIR2, (hit)?1000:0, (hit2)? 1000:0 ); //Send raw data to plotter
    
//    float rawIR  = particleSensor.getIR();
  //  Serial.printf("%f\n", rawIR);
}
