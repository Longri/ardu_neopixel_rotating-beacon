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

#include "Beacon.h"


//                     -  number of LED's
//                     |   -  conected pin
//                     |   |        -  NeoPixel Type
//                     |   |        |               -  LED width for rotating
//                     |   |        |               |
Beacon beacon = Beacon(24, 9, NEO_GRB + NEO_KHZ800, 7);

Timer t1;
int arrLength;
int step = 0;
double second = 3;
BeaconState states[] { BeaconState::ROTATING_ORANGE, BeaconState::BEACON_OFF, BeaconState::ROTATING_RED, BeaconState::BEACON_OFF,
              BeaconState::ROTATING_GREEN, BeaconState::BEACON_OFF, BeaconState::ROTATING_BLUE, BeaconState::BEACON_OFF,
              BeaconState::ON_ORANGE, BeaconState::BEACON_OFF, BeaconState::ON_RED, BeaconState::BEACON_OFF,
              BeaconState::ON_GREEN, BeaconState::BEACON_OFF, BeaconState::ON_BLUE, BeaconState::BEACON_OFF,
              BeaconState::SIGNAL_ORANGE, BeaconState::BEACON_OFF, BeaconState::SIGNAL_RED, BeaconState::BEACON_OFF,
              BeaconState::SIGNAL_GREEN, BeaconState::BEACON_OFF, BeaconState::SIGNAL_BLUE, BeaconState::BEACON_OFF,
              BeaconState::FLASH_ORANGE, BeaconState::BEACON_OFF, BeaconState::FLASH_RED, BeaconState::BEACON_OFF,
              BeaconState::FLASH_GREEN, BeaconState::BEACON_OFF, BeaconState::FLASH_BLUE, BeaconState::BEACON_OFF
};
BeaconState actState;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  //  beacon.setBrightnessInput(A5, 450, 20);
  arrLength = sizeof(states) / sizeof(states[0]);
}


void loop() {
  beacon.loop();

  if (step >= arrLength)return;
  if (!t1 ) {
    actState = states[step];
    t1.start(SECOND, (actState == BeaconState::BEACON_OFF) ? second / 2 : second);
    beacon.setState(actState);
  }

  if (t1.elapsed()  ) {
    t1.reset();
    step++;
  }

}
