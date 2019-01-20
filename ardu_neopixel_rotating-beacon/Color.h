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



#ifndef ROTATING_BEACON_COLOR_H
#define ROTATING_BEACON_COLOR_H

#include <stdint.h>

class Color {
  private:

    uint32_t value;

  public:

    Color(uint8_t r, uint8_t g, uint8_t b): value( ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b) {}

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

//    static const Color GREEN;
//    static const Color RED;
//    static const Color ORANGE;
//    static const Color BLACK;

};

//static const Color Color::GREEN = Color(0, 255, 0);
//static const Color Color::RED = Color(255, 0, 0);
//static const Color Color::ORANGE = Color(255, 50, 0);
//static const Color Color::BLACK = Color(0, 0, 0);

#endif //ROTATING_BEACON_COLOR_H
