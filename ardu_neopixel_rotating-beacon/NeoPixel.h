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

    bool ON = false;

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
      if (loopDeltaTime < 33.579)  return true; // 29 fps
      lastLoopTime = time;
      return false;
    }


    void calculateBrightness() {
      int analogValue = analogRead(brightnessPin);
      float brightness = linearInterpolation( min, max, 0, 1, analogValue );

      if (lastBrightness != brightness) {
        //calculate Color value with brightness changes
        lastBrightness = brightness;
        int8_t r = ((color & 0xff0000) >> 16) * brightness;
        int8_t g = ((color & 0xff00) >> 8) * brightness;
        int8_t b = (color & 0xff) * brightness;
        brightnesColor = Color(r, g, b);
      }
    }

  public:
    RotatingPixel(uint16_t numPixels, uint8_t pin, neoPixelType type, uint8_t pixelWidth)
      : pixels(numPixels, pin, type), numPixels(numPixels), pixelWidth(pixelWidth) {}

    void begin() {
      pixels.begin();
    }

    void on() {
      ON = true;
    }

    void off() {
      ON = false;
      pixels.clear();
      pixels.show();

    }

    void setColor(Color newColor) {
      this->color = newColor;
      brightnesColor = newColor;
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
      if (calculateDeltaTime())return;
      if (brightnessPin >= 0)calculateBrightness();

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
      pixels.show(); // Durchführen der Pixel-Ansteuerung
    }

};

#endif //ROTATING_BEACON_NEOPIXEL_H
