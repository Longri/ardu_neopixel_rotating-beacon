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

   Created by Longri on 20.01.2019.
*/
#ifndef ROTATING_BEACON_TIMER_H
#define ROTATING_BEACON_TIMER_H

enum Unit {
  MILLI, SECOND, MINUTE, HOUR, DAY
};

class Timer {
  private:

    bool running = false;
    unsigned  long durationEnd = 0;


  public:

    void start(Unit unit, int value) {
      if (this->running) return;
      this->running = true;
      unsigned  long now = millis();

      switch (unit) {
        case MILLI:
          durationEnd = now + value;
          break;
        case SECOND:
          durationEnd = now + (value * 1000);
          break;
        case MINUTE:
          durationEnd = now + (value * 1000 * 60);
          break;
        case HOUR:
          durationEnd = now + (value * 1000 * 60 * 60);
          break;
        case DAY:
          durationEnd = now + (value * 1000 * 60 * 60 * 24);
          break;
      }
    }

    operator bool() const {
      if (!this->running)return false;
      unsigned  long now = millis();
      return now > durationEnd;
    }

};




#endif //ROTATING_BEACON_TIMER_H