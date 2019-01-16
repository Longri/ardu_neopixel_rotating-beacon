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

#include <Adafruit_NeoPixel.h>
#include "Color.h"
#include "DoubleAnimator.h"

#ifdef __AVR__
#include <avr/power.h>
#endif


#ifndef ROTATING_BEACON_NEOPIXEL_H
#define ROTATING_BEACON_NEOPIXEL_H

class NeoPixel {

    Adafruit_NeoPixel pixels;
    uint16_t numPixels;
    uint8_t pixelWidth;
    int8_t* pixelArray;
    uint16_t setPixel;

    float loopDeltaTime = 0;
    unsigned long lastLoopTime = millis();

    DoubleAnimator animator;
    double dev = 0;
    float duration = 2000.0f; //default 2sec (2000 msec)
    Color color = Color::RED;

    void calculateDeltaTime() {
      unsigned  long time = millis();
      loopDeltaTime = (time - lastLoopTime);
      lastLoopTime = time;
    }



  public:
    NeoPixel(uint16_t numPixels, uint8_t pin, neoPixelType type, uint8_t pixelWidth)
      : pixels(numPixels, pin, type), numPixels(numPixels), pixelWidth(pixelWidth), pixelArray(new int8_t[pixelWidth]) {}
    void begin() {
      pixels.begin();
    }

    void setColor(Color newColor) {
      this->color = newColor;
    }

    void setRPM(float rpm) {
      duration = 60000.0f / rpm;
    }


    void loop() {
      calculateDeltaTime();

      if (animator.isFinish()) {
        animator.begin(duration, 0.0, 360.0); //one rotate (0 to 360 degrese) at 2 sec;
      }
      animator.update(loopDeltaTime);
      dev = 360.0 / animator.getAct();
      setPixel = (uint16_t)numPixels / dev;

      //clear all Pixels
      for (uint16_t i = 0; i < numPixels; i++) {
        pixels.setPixelColor(i, Color::BLACK);
      }

      for (uint16_t i = 0; i < pixelWidth; i++) {
        pixelArray[i] = setPixel - i;
        if (pixelArray[i] < 0)pixelArray[i] += numPixels;
        pixels.setPixelColor(pixelArray[i], this->color);
      }

      pixels.show(); // Durchführen der Pixel-Ansteuerung

    }

};

#endif //ROTATING_BEACON_NEOPIXEL_H
