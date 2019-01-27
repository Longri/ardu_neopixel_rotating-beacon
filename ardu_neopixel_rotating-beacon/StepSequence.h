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


#include "Timer.h"



class StepSequence {
  private:

    bool run = false;
    int step = 0;
    int stepSize = 0;

    typedef void (*WorkFuncPtr)(void);
    volatile WorkFuncPtr  workFunktions[10];


    void SetCallback(int idx, volatile WorkFuncPtr wf) {
      workFunktions[idx] = wf;
    }
    void TriggerCallback(int idx) {
      if (workFunktions[idx]) {
        workFunktions[idx]();
      }
    }

    typedef void (*ContinusFuncPtr)(bool&);
    volatile ContinusFuncPtr  continusFunktions[10];


    void SetContinusFunktion(int idx, volatile ContinusFuncPtr cf) {
      continusFunktions[idx] = cf;
    }
    bool AskContinus(int idx) {
      if (continusFunktions[idx]) {
        ContinusFuncPtr ptr = continusFunktions[idx];
        bool continusResult;
        ptr(continusResult);
        Serial.print("AskContinus result for Step:");
        Serial.print(idx);
        Serial.print(" is:");
        Serial.println(continusResult);
        return continusResult;
      }
    }

  public:

    StepSequence() {}

    bool isRunning() {
      return this->run;
    }

    void start() {
      Serial.println("");
      Serial.println("");
      Serial.println("START");
      this->run = true;
      if (step == 0)step = 1;
    }

    void stop() {
      Serial.println("STOP");
      this->run = false;
    }

    void reset() {
      Serial.println("RESET");
      this->step = 0;
    }

    void loop() {
      if (!this->run)return;

      //      // get act step continuity contdition and ask
      if (AskContinus(this->step-1)) {
        // continuity condition satisfied, call step work and set new Step
        TriggerCallback(this->step-1);

        step++;
        Serial.print("Stepp++ to:");
        Serial.println(step);
      }

      // break sequens with last Step
      if (this->step >= this->stepSize) {
        stop();
        reset();
      }
    }

    void add(volatile WorkFuncPtr wf, volatile ContinusFuncPtr cf) {
      SetCallback(stepSize, wf)   ;
      SetContinusFunktion(stepSize, cf);
      stepSize++;
    }


};

#endif //ROTATING_BEACON_STTEPSEQUENCE_H
