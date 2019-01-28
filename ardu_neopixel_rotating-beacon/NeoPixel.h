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
#include "Timer.h"

#ifdef __AVR__
#include <avr/power.h>
#endif


#ifndef ROTATING_BEACON_NEOPIXEL_H
#define ROTATING_BEACON_NEOPIXEL_H

enum RotatingState {
  ON, OFF, ROTATE, SIGNAL, FLASH
};

class RotatingPixel {

    Adafruit_NeoPixel pixels;
    uint16_t numPixels;
    uint8_t pixelWidth;
    uint16_t firstPixel;
    uint16_t lastFirstPixel;
    int8_t pixel;

    int brightnessPin = -1;
    int min, max;
    float lastBrightness = -1;

    float loopDeltaTime = 0;
    unsigned long lastLoopTime = millis();

    DoubleAnimator animator;
    double dev = 0;
    float duration = 2000.0f; //default 2sec (2000 msec)
    float brightnessMin = 0.1f;
    float brightnessMax = 1.0f;

    Color color = Color(255, 0, 0);
    Color brightnesColor = Color(0, 0, 0);

    int SIGNAL_CYCLE = 500; // ms
    int FLASH_CYCLE = 75; // ms
    int FLASH_CYCLE_LOW = 775; // ms
    Timer t1;
    Timer t2;
    Timer t3;
    Timer t4;

    RotatingState state = RotatingState::OFF;

    float linearInterpolation(float min, float max, float min2, float max2, float value) {
      float range1 = max - min;
      float range2 = max2 - min2;
      int8_t round1 = ((int)10 * maxBrightness( minBrightness(((value - min) * range2 / range1) + min2, brightnessMax), brightnessMin));
      return  round1 / 10.0f;
    }

    float minBrightness(float a, float b) {
      return (a <= b) ? a : b;
    }

    float maxBrightness(float a, float b) {
      return (a >= b) ? a : b;
    }

    bool calculateDeltaTime() {
      unsigned  long time = millis();
      loopDeltaTime = (time - lastLoopTime);
      if ((state != RotatingState::FLASH)&&loopDeltaTime < 33.579)  return true; // 29 fps
      lastLoopTime = time;
      return false;
    }


    bool calculateBrightness() {
      int analogValue = analogRead(brightnessPin);
      float brightness = linearInterpolation( min, max, 0, 1, analogValue );

      if (lastBrightness != brightness) {
        //calculate Color value with brightness changes
        lastBrightness = brightness;
        int8_t r = ((color & 0xff0000) >> 16) * brightness;
        int8_t g = ((color & 0xff00) >> 8) * brightness;
        int8_t b = (color & 0xff) * brightness;
        brightnesColor = Color(r, g, b);
        return true;
      }
      return false;
    }

  public:
    RotatingPixel(uint16_t numPixels, uint8_t pin, neoPixelType type, uint8_t pixelWidth)
      : pixels(numPixels, pin, type), numPixels(numPixels), pixelWidth(pixelWidth) {}

    void begin() {
      pixels.begin();
    }

    void rotate() {
      state = RotatingState::ROTATE;
      t1.reset();
      t2.reset();
      t3.reset();
      t4.reset();
    }

    void on() {
      state = RotatingState::ON;
      pixels.clear();
      pixels.fill(brightnesColor);
      pixels.show();
      t1.reset();
      t2.reset();
      t3.reset();
      t4.reset();
    }

    void signal() {
      if (!t1) {
        state = RotatingState::SIGNAL;
        pixels.clear();
        pixels.fill(brightnesColor);
        pixels.show();
        t1.start(MILLI, SIGNAL_CYCLE);
      }
    }

    void flash() {
      if (!t1) {
        state = RotatingState::FLASH;
        pixels.clear();
        pixels.fill(brightnesColor);
        pixels.show();
        t1.start(MILLI, FLASH_CYCLE);
      }
    }

    void off() {
      state = RotatingState::OFF;
      pixels.clear();
      pixels.show();
      t1.reset();
      t2.reset();
      t3.reset();
      t4.reset();
    }

    void setColor(Color newColor) {
      this->color = newColor;
      brightnesColor = newColor;
      if (state == RotatingState::ON) {
        pixels.clear();
        pixels.fill(brightnesColor);
        pixels.show();
      }
    }

    void setRPM(float rpm) {
      duration = 60000.0f / rpm;
    }

    void setBrightnessInput(int pin, int minValue, int maxValue) {
      brightnessPin = pin;
      min = minValue;
      max = maxValue;
    }


    void loop() {
      if (state == RotatingState::OFF)return;
      if (calculateDeltaTime())return;
      bool brightnessChanged = false;
      if (brightnessPin >= 0) {
        brightnessChanged = calculateBrightness();
      }

      switch (state) {
        case RotatingState::OFF:
          return;

        case RotatingState::ROTATE:
          if (animator.isFinish()) {
            animator.begin(duration, 0.0, 360.0); //one rotate (0 to 360 degrese) at 2 sec;
          }
          animator.update(loopDeltaTime);
          dev = 360.0 / animator.getAct();
          firstPixel = (uint16_t)numPixels / dev;

          if (lastFirstPixel == firstPixel)return;

          //clear all Pixels
          pixels.clear();

          for (uint16_t i = 0; i < pixelWidth; i++) {
            int8_t pixel = firstPixel - i;
            if (pixel < 0)pixel += numPixels;
            pixels.setPixelColor(pixel, this->brightnesColor);
          }
          break;

        case RotatingState::ON:
          if (brightnessChanged) {
            pixels.fill(this->brightnesColor);
          }
          break;

        case RotatingState::SIGNAL:
          if (!t1) {
            pixels.clear();
            pixels.fill(brightnesColor);
            t1.start(MILLI, SIGNAL_CYCLE);
            break;
          }
          if (t1.elapsed() && !t2) {
            pixels.clear();
            t2.start(MILLI, SIGNAL_CYCLE);
            break;
          }
          if (t2.elapsed()) {
            t1.reset();
            t2.reset();
            t3.reset();
            t4.reset();
          }
          break;
        case RotatingState::FLASH:
          if (!t1) {
            pixels.clear();
            pixels.fill(brightnesColor);
            t1.start(MILLI, FLASH_CYCLE);
            break;
          }
          if (t1.elapsed() && !t2) {
            pixels.clear();
            t2.start(MILLI, FLASH_CYCLE);
            break;
          }
          if (t2.elapsed() && !t3) {
            pixels.clear();
            pixels.fill(brightnesColor);
            t3.start(MILLI, FLASH_CYCLE);
            break;
          }
          if (t3.elapsed() && !t4) {
            pixels.clear();
            t4.start(MILLI, FLASH_CYCLE_LOW);
            break;
          }
          if (t4.elapsed()) {
            t1.reset();
            t2.reset();
            t3.reset();
            t4.reset();
          }
          break;
      }
      pixels.show(); // Durchführen der Pixel-Ansteuerung
    }

};

#endif //ROTATING_BEACON_NEOPIXEL_H
