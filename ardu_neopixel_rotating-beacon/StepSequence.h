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
#ifndef ROTATING_BEACON_STTEPSEQUENCE_H
#define ROTATING_BEACON_STTEPSEQUENCE_H

class StepSequence {
  private:

    bool run = false;

  public:

    StepSequence() {}

    void start() {
      this->run = true;
    }

    void stop() {
      this->run = false;
    }

    void loop() {}
};

#endif //ROTATING_BEACON_STTEPSEQUENCE_H
