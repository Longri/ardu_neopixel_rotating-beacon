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

   Created by Longri on 16.01.2019.
*/

#ifndef ROTATING_BEACON_DOUBLEANIMATOR_H
#define ROTATING_BEACON_DOUBLEANIMATOR_H

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

#endif //ROTATING_BEACON_DOUBLEANIMATOR_H
