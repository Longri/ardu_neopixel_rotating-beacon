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

   Created by Longri on 17.01.2019.
*/

#include <Adafruit_NeoPixel.h>
#include "State.h"
//#include "Color.h"
#include "NeoPixel.h"


#ifndef BEACON_H
#define BEACON_H

class Beacon {

    NeoPixel rotate;
    State state = State::OFF;

  public:
    Beacon(uint16_t numPixels, uint8_t pin, neoPixelType type, uint8_t pixelWidth)
    : rotate(numPixels, pin, type, pixelWidth)
    {}


    void setState(State newState) {
      switch (newState) {
        case State::OFF:
          rotate.off();
          break;
        case State::ROTATING_ORANGE:
        case State::ROTATING_RED:
        case State::ROTATING_GREEN:
          rotate.on();
          switch (newState) {
            case State::ROTATING_ORANGE:
              rotate.setColor(Color::ORANGE);
              break;
            case State::ROTATING_RED:
             rotate.setColor(Color::RED);
              break;
            case State::ROTATING_GREEN:
             rotate.setColor(Color::GREEN);
              break;
          }
          break;
      }
    }

    void loop() {

      switch (this->state) {
        case State::OFF:
          return;
        case State::ROTATING_ORANGE:
        case State::ROTATING_RED:
        case State::ROTATING_GREEN:
          rotate.loop();
          break;
      }

    }


};

#endif //BEACON_H
