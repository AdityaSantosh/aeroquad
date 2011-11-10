/*
  AeroQuad v3.0 - March 2011
  www.AeroQuad.com
  Copyright (c) 2011 Ted Carancho.  All rights reserved.
  An Open Source Arduino based multicopter.
 
  This program is free software: you can redistribute it and/or modify 
  it under the terms of the GNU General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or 
  (at your option) any later version. 

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
  GNU General Public License for more details. 

  You should have received a copy of the GNU General Public License 
  along with this program. If not, see <http://www.gnu.org/licenses/>. 
*/

#include <Wire.h>             // @see Kenny, Arduino IDE compiliation bug

#include <AQMath.h>
#include <Device_I2C.h>
#include <Gyroscope_IDG_IDZ500.h>
#include <Axis.h>

unsigned long timer;

void setup()
{
  Serial.begin(115200);
  Serial.println("Gyroscope library test (IDG_IDZ500)");

  setGyroAref(3.3);
  initializeGyro();
  calibrateGyro();
  timer = millis();
}

void loop(void) 
{
  if((millis() - timer) > 10) // 100Hz
  {
    timer = millis();
    measureGyro();
    
    Serial.print("Roll: ");
    Serial.print(degrees(gyroRate[ROLL]));
    Serial.print(" Pitch: ");
    Serial.print(degrees(gyroRate[PITCH]));
    Serial.print(" Yaw: ");
    Serial.print(degrees(gyroRate[YAW]));
    Serial.print(" Heading: ");
    Serial.print(degrees(gyroHeading));
    Serial.println();
  }
}

