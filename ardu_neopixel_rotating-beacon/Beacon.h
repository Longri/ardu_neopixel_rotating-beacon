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
#include "Color.h"

class Beacon {
    RotatingPixel rotatePixel;
    BeaconState state = BeaconState::BEACON_OFF;

  public:

    Beacon(uint16_t numPixels, uint8_t pin, neoPixelType type, uint8_t pixelWidth): rotatePixel(RotatingPixel(numPixels, pin, type, pixelWidth)) {
      rotatePixel.begin();
      Color color(5, 0, 0);
      rotatePixel.setColor(color);
      rotatePixel.setRPM(180);
    }

    void setBrightnessInput(int pin, int minValue, int maxValue) {
      rotatePixel.setBrightnessInput(A5, 450, 20);
    }

    void setState(BeaconState newState) {
      switch (newState) {
        case BeaconState::BEACON_OFF:
          Serial.println("setState OFF");
          rotatePixel.off();
          break;
        case BeaconState::ON_ORANGE:
          rotatePixel.on();
          rotatePixel.setColor(Color::ORANGE);
          break;
        case BeaconState::ON_RED:
          rotatePixel.on();
          rotatePixel.setColor(Color::RED);
          break;
        case BeaconState::ON_GREEN:
          rotatePixel.on();
          rotatePixel.setColor(Color::GREEN);
          break;
        case BeaconState::ON_BLUE:
          rotatePixel.on();
          rotatePixel.setColor(Color::BLUE);
          break;
        case BeaconState::ROTATING_ORANGE:
          rotatePixel.rotate();
          rotatePixel.setColor(Color::ORANGE);
          break;
        case BeaconState::ROTATING_RED:
          rotatePixel.rotate();
          rotatePixel.setColor(Color::RED);
          break;
        case BeaconState::ROTATING_GREEN:
          rotatePixel.rotate();
          rotatePixel.setColor(Color::GREEN);
          break;
        case BeaconState::ROTATING_BLUE:
          rotatePixel.rotate();
          rotatePixel.setColor(Color::BLUE);
          break;

      }
      this->state = newState;
    }

    void loop() {
      switch (this->state) {
        case BeaconState::ROTATING_ORANGE:
        case BeaconState::ROTATING_RED:
        case BeaconState::ROTATING_GREEN:
        case BeaconState::ROTATING_BLUE:
        case BeaconState::ON_ORANGE:
        case BeaconState::ON_RED:
        case BeaconState::ON_GREEN:
        case BeaconState::ON_BLUE:
          rotatePixel.loop();
          break;
      }
    }

};

#endif //BEACON_H
