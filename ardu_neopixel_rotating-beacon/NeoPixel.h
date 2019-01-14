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

#ifdef __AVR__
#include <avr/power.h>
#endif


#ifndef GREENHOUSE_NEOPIXEL_H
#define GREENHOUSE_NEOPIXEL_H

class NeoPixel {
    Adafruit_NeoPixel pixels;
    int pause = 1000; // 100 Millisekunden Pause bis zur Ansteuerung der nächsten LED.
  public:
    NeoPixel(uint16_t numPixels, uint8_t pin, neoPixelType type) : pixels(numPixels, pin, type) {}
    void begin() {
      pixels.begin();
    }



    void loop() {
      Serial.println("NeoPixel Loop entry");
      pixels.setPixelColor(1, pixels.Color(0, 10, 0)); // Pixel1 leuchtet in der Farbe Grün
      pixels.show(); // Durchführen der Pixel-Ansteuerung
      delay (pause); // Pause, in dieser Zeit wird nichts verändert.

      // Zurücksetzen aller Pixelfarben auf Stufe "0" (aus)
      pixels.setPixelColor(1, pixels.Color(0, 0, 0));
      pixels.setPixelColor(2, pixels.Color(0, 0, 0));
      pixels.setPixelColor(3, pixels.Color(0, 0, 0));
      pixels.setPixelColor(4, pixels.Color(0, 0, 0));
      pixels.setPixelColor(5, pixels.Color(0, 0, 0));
      pixels.show(); // Durchführen der Pixel-Ansteuerung
      delay (pause); // Pause, die LEDs bleiben in dieser Zeit aus

    }

};


#endif //GREENHOUSE_NEOPIXEL_H
