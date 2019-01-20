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


#include "SequenceFunctionArray.h"
#include "Timer.h"
#include <Function.h>



class StepSequence {
  private:

    bool run = false;
    int step = 0;

    SequenceFunctionArray  continuityContditions;
    SequenceFunctionArray  stepWorks;

  public:

    StepSequence() {}

    void start() {
      this->run = true;
      if (step == 0)step = 1;
    }

    void stop() {
      this->run = false;
    }

    void reset() {
      this->step = 0;
    }

    void loop() {
      if (!this->run)return;

      // get act step continuity contdition and ask
      if (continuityContditions[this->step]) {
        // continuity condition satisfied, call step work and set new Step
        step++;
        stepWorks[this->step]();
      }

      // break sequens with last Step
      if (this->step >= stepWorks.getSize()) {
        stop();
        reset();
      }
    }

    void add(vl::Func<bool ()> const &continuityContdition, vl::Func<bool ()> const &stepWork) {
      continuityContditions.add(continuityContdition);
      stepWorks.add(stepWork);
    }
};

#endif //ROTATING_BEACON_STTEPSEQUENCE_H
