#include <Arduino.h>

const int Pin = 13;
void StimOn(double rate, double totalDuration, double blockOnDur,
            double stimDur, double blockOffDur);

void setup()
{
  Serial.begin(9600);
  pinMode(Pin, OUTPUT);
}

void loop()
{
  double rate = 5;           // rate in hz
  double totalDuration = 30; // duration of stim
  double stimDur = 5;        // in ms
  double blockOnDur = 0.5;   // in min
  double blockOffDur = 0.5;  // in min
  static bool haveIRun = false;

  if (!haveIRun)
  {
    StimOn(rate, totalDuration, blockOnDur,
           stimDur, blockOffDur);
    haveIRun = true;
    Serial.println("I am done");
  }
}

void StimOn(double rate, double totalDuration, double blockOnDur,
            double stimDur, double blockOffDur)
{

  // how many blocks will there be?
  int numBlocks = totalDuration / (blockOnDur + blockOffDur);

  // within each block, how many times does the laser turn on?
  double numCycles_min = rate * 60;
  double numCycles_Block = numCycles_min * blockOnDur;

  // within each cycle, how long are the on and off periods?
  double totalPeriod = 1000 / rate;
  double offPeriod = totalPeriod - stimDur;

  // block off dur in ms
  double blockOffDur_ms = blockOffDur * 60 * 1000;

  // loop over the blocks
  while (numBlocks >= 0)
  {
    // stimulate for the on portion of the block
    int numCyclesLeft = numCycles_Block;
    while (numCyclesLeft >= 0)
    {
      digitalWrite(Pin, HIGH);
      delay(stimDur);
      digitalWrite(Pin, LOW);
      delay(offPeriod);

      numCyclesLeft -= 1;
    }
    // stim off
    digitalWrite(Pin, LOW);
    delay(blockOffDur_ms);
  }
}