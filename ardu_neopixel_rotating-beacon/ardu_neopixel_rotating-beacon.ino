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
  Serial.begin(9600);
  beacon.setBrightnessInput(A5, 450, 20);


  // Sequence Step1   #######################
  sequence.add([]() { //Step1: continuity contdition
    return t3;
  }, []() {           //Step1: step work
    t4.start(MINUTE, 0.1);
    beacon.setState(ROTATING_GREEN);
    return true;
  });
  // Sequence Step1   #######################

  // Sequence Step2   #######################
  sequence.add([]() { //Step2: continuity contdition
    return t4;
  }, []() {           //Step2: step work
    t1.reset();
    t2.reset();
    t3.reset();
    t4.reset();
    beacon.setState(OFF);
    return true;
  });
  // Sequence Step2   #######################
}


void loop() {
  beacon.loop();
  sequence.loop();

  if (!t1) {
    t1.start(MILLI, 1000);
    beacon.setState(OFF);
  } else {
    if (!t2) {
      t2.start(SECOND, 1);
      beacon.setState(ROTATING_RED);
    } else {
      if (!t3) {
        t3.start(MINUTE, 0.1);
      } else {
        //        t1.reset();
        //        t2.reset();
        //        t3.reset();
      }
      beacon.setState(OFF);
      sequence.start();
    }
  }

}
