/*
   Copyright 2019 Longri

   This program is free software: you can redistribute it and/or modify it under the
   terms of the GNU Lesser General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
   PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License along with
   this program. If not, see <http://www.gnu.org/licenses/>.

   Created by Longri on 14.01.2019.
*/

#include "Color.h"
#include "NeoPixel.h"
#include "Beacon.h"
#include "StepSequence.h"
#include "Timer.h"
#include "SequenceFunctionArray.h"


//                     -  number of LED's
//                     |   -  conected pin
//                     |   |        -  NeoPixel Type
//                     |   |        |               -  LED width for rotating
//                     |   |        |               |
Beacon beacon = Beacon(24, 9, NEO_GRB + NEO_KHZ800, 7);


StepSequence sequence = StepSequence();
Timer t1;
Timer t2;
Timer t3;
Timer t4;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  beacon.setBrightnessInput(A5, 450, 20);


  // Sequence Step1   #######################
  sequence.add([]() { //Step1: continuity contdition
    Serial.print("ASK STEP 1/ return:");
    Serial.println(true);
    return true;
  }, []() {           //Step1: step work
    Serial.println("Call work for Step 1");
    if (t1.start(SECOND, 30));
    beacon.setState(ON_GREEN);
  });
  // Sequence Step1   #######################

  // Sequence Step2   #######################
  sequence.add([]() { //Step2: continuity contdition
    Serial.print("ASK STEP 2/ return:");
    Serial.println(t1.elapsed());
    return t1.elapsed();
  }, []() {           //Step2: step work
    Serial.println("Call work for Step 2");
    if (t2.start(SECOND, 100));
    beacon.setState(ROTATING_GREEN);
  });
  // Sequence Step2   #######################

  // Sequence Step3   #######################
  sequence.add([]() { //Step3: continuity contdition
    Serial.print("ASK STEP 3/ return:");
    Serial.println(t2.elapsed());
    return t2.elapsed();
  }, []() {           //Step3: step work
    Serial.println("Call work for Step 3");
    beacon.setState(OFF);
  });
  // Sequence Step3   #######################
}


void loop() {
  beacon.loop();
  sequence.loop();

  if (!sequence.isRunning()) {
    Serial.println("Sequence start from Loop()");
    sequence.start();
  }
}
