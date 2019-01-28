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

#include <Adafruit_NeoPixel.h>
#include <stdint.h>

#ifdef __AVR__
#include <avr/power.h>
#endif


enum Unit {
  MILLI, SECOND, MINUTE, HOUR, DAY
};

enum RotatingState {
  ON, OFF, ROTATE, SIGNAL, FLASH
};

enum BeaconState {
  ROTATING_ORANGE,
  ROTATING_RED,
  ROTATING_GREEN,
  ROTATING_BLUE,

  FLASH_ORANGE,
  FLASH_RED,
  FLASH_GREEN,
  FLASH_BLUE,

  SIGNAL_ORANGE,
  SIGNAL_RED,
  SIGNAL_GREEN,
  SIGNAL_BLUE,

  ON_ORANGE,
  ON_RED,
  ON_GREEN,
  ON_BLUE,

  BEACON_OFF
};


class Timer {
  private:

    bool running = false;
    unsigned  long durationEnd = 0;


  public:

    void reset() {
      this->durationEnd = 0;
      this->running = false;
    }

    bool start(Unit unit, double value) {
      if (this->running) return false;
      this->running = true;
      unsigned  long now = millis();

      switch (unit) {
        case MILLI:
          durationEnd = now + value;
          break;
        case SECOND:
          durationEnd = now + ((int)(value * 1000.0));
          break;
        case MINUTE:
          durationEnd = now + ((int)(value * 1000.0 * 60.0));
          break;
        case HOUR:
          durationEnd = now + ((int)(value * 1000.0 * 60.0 * 60.0));
          break;
        case DAY:
          durationEnd = now + ((int)(value * 1000.0 * 60.0 * 60.0 * 24.0));
          break;
      }
      return true;
    }

    bool elapsed() const {
      if (!this->running)return false;
      unsigned  long now = millis();
      return now > durationEnd;
    }

    operator bool() const {
      return this->running;
    }

};


class DoubleAnimator {
    double start, end, act;
    float time;
    float duration;
    bool finish;

    /**
       http://stackoverflow.com/questions/356807/java-double-comparison-epsilon
       <p>
       Returns true if two doubles are considered equal. Tests if the absolute
       difference between the two doubles has a difference less then a given
       double (epsilon). Determining the given epsilon is highly dependant on the
       precision of the doubles that are being compared.

       @param a         double to compare.
       @param b         double to compare
       @param precision double which is compared to the absolute difference of two
                        doubles to determine if they are equal.
       @return true if a is considered equal to b.
    */
    static boolean equals(double a, double b, double precision) {
      return a == b || abs(a - b) < precision;
    }

  public:
    DoubleAnimator() {
      finish = true;
    }

    void begin(float duration, double start, double end) {
      begin(duration, start, end, 0);
    }

    void begin(float duration, double start, double end, double precision) {
      if (equals(start, end, precision)) return;
      this->duration = duration;
      this->start = start;
      this->end = end;
      this->time = 0;
      this->finish = false;
    }

    bool update(float delta) {
      if (finish) return false;
      time += delta;
      boolean complete = time >= duration;
      float percent;
      if (complete) {
        act = end;
        finish = true;
        return true;
      } else {
        percent = time / duration;
      }
      act = start + (end - start) * percent;
      return true;
    }

    double getAct() {
      return this->act;
    }

    bool isFinish() {
      return this->finish;
    }

};


class Color {
  private:

    uint32_t value;

  public:

    Color(uint8_t r, uint8_t g, uint8_t b): value( ((uint32_t)r << 16) | ((uint32_t)g <<  8) | (uint32_t)b) {}

    Color(uint32_t value): value(value) {}

    Color &operator=(uint32_t other) {
      this->value = other;
      return *this;
    }

    Color &operator=(Color other) {
      this->value = other.value;
      return *this;
    }

    operator uint32_t() const {
      return this->value;
    }

    static Color GREEN;
    static Color RED;
    static Color ORANGE;
    static Color BLACK;
    static Color BLUE;

};

Color Color::GREEN = Color(0, 255, 0);
Color Color::RED = Color(255, 0, 0);
Color Color::ORANGE = Color(255, 50, 0);
Color Color::BLACK = Color(0, 0, 0);
Color Color::BLUE = Color(0, 0, 200);

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
      if ((state != RotatingState::FLASH) && loopDeltaTime < 33.579)  return true; // 29 fps
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
      if ( this->state == newState)return;
      switch (newState) {
        case BeaconState::BEACON_OFF:
          rotatePixel.off();
          Serial.println("Set Beacon state: BEACON_OFF");
          break;
        case BeaconState::ON_ORANGE:
          rotatePixel.setColor(Color::ORANGE);
          rotatePixel.on();
          Serial.println("Set Beacon state: ON_ORANGE");
          break;
        case BeaconState::ON_RED:
          rotatePixel.setColor(Color::RED);
          rotatePixel.on();
          Serial.println("Set Beacon state: ON_RED");
          break;
        case BeaconState::ON_GREEN:
          rotatePixel.setColor(Color::GREEN);
          rotatePixel.on();
          Serial.println("Set Beacon state: ON_GREEN");
          break;
        case BeaconState::ON_BLUE:
          rotatePixel.setColor(Color::BLUE);
          rotatePixel.on();
          Serial.println("Set Beacon state: ON_BLUE");
          break;
        case BeaconState::ROTATING_ORANGE:
          rotatePixel.setColor(Color::ORANGE);
          rotatePixel.rotate();
          Serial.println("Set Beacon state: ROTATING_ORANGE");
          break;
        case BeaconState::ROTATING_RED:
          rotatePixel.setColor(Color::RED);
          rotatePixel.rotate();
          Serial.println("Set Beacon state: ROTATING_RED");
          break;
        case BeaconState::ROTATING_GREEN:
          rotatePixel.setColor(Color::GREEN);
          rotatePixel.rotate();
          Serial.println("Set Beacon state: ROTATING_GREEN");
          break;
        case BeaconState::ROTATING_BLUE:
          rotatePixel.setColor(Color::BLUE);
          rotatePixel.rotate();
          Serial.println("Set Beacon state: ROTATING_BLUE");
          break;
        case BeaconState::SIGNAL_ORANGE:
          rotatePixel.setColor(Color::ORANGE);
          rotatePixel.signal();
          Serial.println("Set Beacon state: SIGNAL_ORANGE");
          break;
        case BeaconState::SIGNAL_RED:
          rotatePixel.setColor(Color::RED);
          rotatePixel.signal();
          Serial.println("Set Beacon state: SIGNAL_RED");
          break;
        case BeaconState::SIGNAL_GREEN:
          rotatePixel.setColor(Color::GREEN);
          rotatePixel.signal();
          Serial.println("Set Beacon state: SIGNAL_GREEN");
          break;
        case BeaconState::SIGNAL_BLUE:
          rotatePixel.setColor(Color::BLUE);
          rotatePixel.signal();
          Serial.println("Set Beacon state: SIGNAL_BLUE");
          break;





        case BeaconState::FLASH_ORANGE:
          rotatePixel.setColor(Color::ORANGE);
          rotatePixel.flash();
          Serial.println("Set Beacon state: FLASH_ORANGE");
          break;
        case BeaconState::FLASH_RED:
          rotatePixel.setColor(Color::RED);
          rotatePixel.flash();
          Serial.println("Set Beacon state: FLASH_RED");
          break;
        case BeaconState::FLASH_GREEN:
          rotatePixel.setColor(Color::GREEN);
          rotatePixel.flash();
          Serial.println("Set Beacon state: FLASH_GREEN");
          break;
        case BeaconState::FLASH_BLUE:
          rotatePixel.setColor(Color::BLUE);
          rotatePixel.flash();
          Serial.println("Set Beacon state: FLASH_BLUE");
          break;

      }
      this->state = newState;
    }

    void loop() {
      rotatePixel.loop();
    }

};


#endif //BEACON_H
