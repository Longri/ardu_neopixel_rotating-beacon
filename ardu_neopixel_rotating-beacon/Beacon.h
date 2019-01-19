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

#ifndef BEACON_H
#define BEACON_H

#include "State.h"

class Beacon {
    NeoPixel rotatePixel;
    BeaconState state = BeaconState::OFF;

  public:

    Beacon(uint16_t numPixels, uint8_t pin, neoPixelType type, uint8_t pixelWidth): rotatePixel(NeoPixel(numPixels, pin, type, pixelWidth)) {
      rotatePixel.begin();
      Color color(5, 0, 0);
      rotatePixel.setColor(color);
      rotatePixel.setRPM(25);
      rotatePixel.setRPM(180);
      rotatePixel.setBrightnessInput(A5, 450, 20);
    }

    void setState(BeaconState newState) {
      switch (newState) {
        case OFF:
          Serial.println("setState OFF");
          rotatePixel.off();
          break;
        case ROTATING_ORANGE:
        case ROTATING_RED:
        case ROTATING_GREEN:
          Serial.println("setState Rotate");
          rotatePixel.on();
          switch (newState) {
            case ROTATING_ORANGE:
              rotatePixel.setColor(Color::ORANGE);
              break;
            case ROTATING_RED:
              rotatePixel.setColor(Color::RED);
              break;
            case ROTATING_GREEN:
              rotatePixel.setColor(Color::GREEN);
              break;
          }
          break;
      }
      this->state = newState;
    }

    void loop() {
      switch (this->state) {
        case OFF:
          return;
        case ROTATING_ORANGE:
        case ROTATING_RED:
        case ROTATING_GREEN:
          rotatePixel.loop();
          break;
      }
    }

};

#endif //BEACON_H
