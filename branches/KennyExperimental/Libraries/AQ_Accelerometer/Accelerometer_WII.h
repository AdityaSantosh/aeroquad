/*
  AeroQuad v3.0 - May 2011
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

#ifndef _AEROQUAD_ACCELEROMETER_WII_H_
#define _AEROQUAD_ACCELEROMETER_WII_H_

#include <Accelerometer.h>
#include <Platform_Wii.h>

Platform_Wii *accelPlatformWii;

void initializeAccel() {
  accelScaleFactor = 0.09165093;  // Experimentally derived to produce meters/s^2 
}

void measureAccel() {
//  int accelADC[3];
  meterPerSec[XAXIS] = accelPlatformWii->getAccelADC(XAXIS) - accelZero[XAXIS];
  meterPerSec[YAXIS] = accelZero[YAXIS] - accelPlatformWii->getAccelADC(YAXIS);
  meterPerSec[ZAXIS] = accelZero[ZAXIS] - accelPlatformWii->getAccelADC(ZAXIS);
//  for (byte axis = XAXIS; axis < LASTAXIS; axis++) {
//    meterPerSec[axis] = filterSmooth(accelADC[axis] * accelScaleFactor, meterPerSec[axis], accelSmoothFactor);
//  }
}

void measureAccelSum() {
  // do nothing here since it's already oversample in the APM_ADC class
}

void evaluateMeterPerSec() {
  // do nothing here since it's already oversample in the APM_ADC class
}

void calibrateAccel() {
  int findZero[FINDZERO];

  for(byte calAxis = XAXIS; calAxis < LASTAXIS; calAxis++) {
    for (int i=0; i<FINDZERO; i++) {
      accelPlatformWii->measure();
      findZero[i] = accelPlatformWii->getAccelADC(calAxis);
      delay(5);
    }
    accelZero[calAxis] = findMedianInt(findZero, FINDZERO);
  }
    
  // store accel value that represents 1g
  accelOneG = -meterPerSec[ZAXIS];
  // replace with estimated Z axis 0g value
  accelZero[ZAXIS] = (accelZero[XAXIS] + accelZero[YAXIS]) / 2;
}


#endif