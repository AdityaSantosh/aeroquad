/*
  AeroQuad v1.6 - March 2010
  www.AeroQuad.com
  Copyright (c) 2010 Ted Carancho.  All rights reserved.
  An Open Source Arduino based quadrocopter.
 
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

// This class is responsible for calculating vehicle attitude

#include "SubSystem.h"
#include "Filter.h"
#include <math.h>

class Attitude_CompFilter: public SubSystem {
private:
  float dt;
  
  // Sensor Filter
  float flightAngle[2] = {0,0};
  float timeConstant; // Read in from EEPROM

  // Complementary roll/pitch angle
  float filterTermRoll[4] = {0,0,0,0};
  float filterTermPitch[4] = {0,0,0,0};
  
  // Heading
  float heading;
  float headingScaleFactor;
  Filter filterHeading;

  float CompFilter(float previousAngle, float newAngle, float newRate, float filterTerm, float dt) {
    // Written by RoyLB at http://www.rcgroups.com/forums/showpost.php?p=12082524&postcount=1286    
    filterTerm[0] = (newAngle - previousAngle) * timeConstant * timeConstant;
    filterTerm[2] += filterTerm[0] * dt;
    filterTerm[1] = filterTerm[2] + (newAngle - previousAngle) * 2 * timeConstant + newRate;
    return (filterTerm[1] * dt) + previousAngle;
  }

public:
  //Required methods to impliment for a SubSystem
  Attitude_CompFilter():SubSystem() {
    //Perform any initalization of variables you need in the constructor of this SubSystem
    timeConstant = eeprom.read(FILTERTERM_ADR);
    filterHeading.initialize(eeprom.read(HEADINGSMOOTH_ADR));
  }

  void initialize(unsigned int frequency, unsigned int offset = 0) {
    //Call the parent class' _initialize to setup all the frequency and offset related settings
    this->_initialize(frequency, offset);

    //Perform any custom initalization you need for this SubSystem
    dt = frequency / 1000.0; 
    flightAngle[ROLL] = sensors.getAngleDegrees(ROLL);
    flightAngle[PITCH] = sensors.getAngleDegrees(PITCH);
    filterTermRoll[2] = -sensors.getRateDegPerSec(ROLL);
    filterTermPitch[2] = -sensors.getRateDegPerSec(PITCH);
    headingScaleFactor = sensors.getGyroScaleFactor();
  }
  
  void process(unsigned long currentTime) {
    //Check to see if this SubSystem is allowed to run
    //The code in _canProcess checks to see if this SubSystem is enabled and its been long enough since the last time it ran
    //_canProcess also records the time that this SubSystem ran to use in future timing checks.
    if (this->_canProcess(currentTime)) {
      // Calculate absolute angle of vehicle
      flightAngle[ROLL] = CompFilter(flightAngle[ROLL], sensors.getAngleDegrees(ROLL), sensors.getRateDegPerSec(ROLL), filterTermRoll, dt);
      flightAngle[PITCH] = CompFilter(flightAngle[PITCH], sensors.getAngelDegrees(PITCH), sensors.getRateDegPerSec(PITCH), filterTermPitch, dt);
      // Calculate heading from yaw gyro
      heading = filterHeading.smooth(sensors.getGyro(YAW) * headingScaleFactor * dT);
    }
  }
  
  float getFlightAngle(byte axis) return flightAngle[axis];
  float getHeading(void) return heading;
  void setTimeConstant(float value) timeConstant = value;
  float getTimeConstant(void) return timeConstant;
  float getFilterSmoothFactor(void) return filterHeading.getSmoothFactor();
};