// Temperature, Pressure, Altitude, UV intensity and Soil moisture

#include <LiquidCrystal.h>
#include <Adafruit_BMP280.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Hardware pin definitions
int UVOUT = A0; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board
int rainPin = A2;

Adafruit_BMP280   bmp; // I2C

// you can adjust the threshold value
int thresholdValue = 800;
 
void setup()
{
  pinMode(rainPin, INPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
 
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
 
  Serial.println("ML8511 example");

  Serial.println(F("BMP280   test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find   a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default   settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /*   Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp.   oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure   oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering.   */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
}
 
void loop()
{
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
 
  //Serial.print("output: ");
  //Serial.print(refLevel);
  //Serial.print("ML8511 output: ");
  //Serial.print(uvLevel);
  //Serial.print(" / ML8511 voltage: ");
  //Serial.print(outputVoltage);

  float f = bmp.readTemperature(); //*9/5 + 32 ; //Conversion   from C to F
  Serial.print(F("Temperature = "));    
  Serial.print(f);
  Serial.println(" *C");

  if (f <  18) {
    Serial.println("***Low Temperature***");
  } 
  if (f >  24) {
    Serial.println("***High Temperature***");
  } 
  if (f >  18 && f < 24) {
    Serial.println("Ideal Temperature!");
  } 
  Serial.println("");
    
  float P = bmp.readPressure()/3386.39;    //displaying the Pressure in inches of mercury, you can change the unit
  Serial.print(F("Pressure   = "));
  Serial.print(P);
  Serial.println(" inHg");

  float   A = bmp.readAltitude(1030)*3.28084;   //The "1019.66" is the pressure(hPa)   at sea level in day in your region
  Serial.print(F("Approx altitude = "));         //If you don't know it, modify it until you get your current altitude
  float mm = A*0.3048;
  Serial.print(mm);
  Serial.println(" m");
  Serial.println();
 
  Serial.print("UV Intensity: ");
  Serial.print(uvIntensity);
  Serial.println(" mW/cm^2");  
  //lcd.clear();
  //lcd.print("UV Ray Intensity");
  //lcd.setCursor(0, 1);
  //lcd.print(uvIntensity);
  //lcd.print(" mW/cm^2");
  
  Serial.println();

  // read the input on analog pin 0:
  int sensorValue = analogRead(rainPin);
  Serial.print("Soil moisture sensor value: ");
  Serial.println(sensorValue);
  if(sensorValue < thresholdValue){
    Serial.println("Doesn't need watering!");
  }
  else {
    Serial.println("***Time to water your plants***");
  }
  Serial.println();
  delay(3000);

}
 
//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
 
  return(runningValue);
}
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
