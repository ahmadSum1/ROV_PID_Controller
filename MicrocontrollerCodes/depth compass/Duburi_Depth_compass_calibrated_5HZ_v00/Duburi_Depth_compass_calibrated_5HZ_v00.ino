#include <Wire.h>
#include "Adafruit_BMP280.h"
#include "Adafruit_HMC5883_U.h"

#define c1 1336
#define c2 - 3.6761
#define c3 -4376.8964

#define m1 -0.01322
#define m2 0.001596
#define a 0.04316
#define b 0.006805

double altitude1 = 0;
double altitude2 = 0;
double altitude3 = 0;

Adafruit_BMP280 bme;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

float current_head = 0;


void setup() {

  Serial.begin(9600);
  bme.begin();
  mag.begin();
  Wire.begin();

  delay(1000);

}

void loop() {
  presss();
  compasss();
  //  String data = (String)altitude1 + "," +(String)altitude2 + "," +(String)altitude3 + "," + (String)current_head + ",";
  String data = (String)altitude1 + "," + (String)current_head + ",";
  Serial.println(data);
  delay(200);


}


void presss() {
  //   Serial.print("Pressure = ");
  //    Serial.print(bme.readPressure());
  //    Serial.print(" Pa     ");
  //  float current_alt = bme.readAltitude(1013.25);
  double x1 = bme.readPressure();
  double x2 = bme.readAltitude(1013.25);
  altitude1 = m1 * x1 + c1;                               //using pressuse data
  //  altitude2 = m2 * x2 + c2;                               // altitude = -3.6761 + 0.001596 * bme.readAltitude(1013.25)(altitude air)
  //  altitude3 = c3 + a * x1 + b * x2;  //Y = -4376.8964 + 0.04316 * X1 + 0.006805 * X2
}
void compasss()
{
  sensors_event_t event;
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float declinationAngle = -0.15;
  heading += declinationAngle;
  // Correct for when signs are reversed.
  if (heading < 0)
    heading += 2 * PI;
  // Check for wrap due to addition of declination.
  if (heading > 2 * PI)
    heading -= 2 * PI;
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180 / M_PI;
  if (headingDegrees > 180) {
    current_head = (headingDegrees - 360);
  }
  if (headingDegrees <= 180) {
    current_head = headingDegrees;
  }

}
