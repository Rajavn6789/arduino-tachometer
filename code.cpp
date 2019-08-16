/*
 * SIMPLE ARDUINO BIKE TACHOMETER
 */

const int TOTAL_LEDS = 24;
const int LED_UPDATE_INTERVAL = 200;
const int MAX_RPM = 8000;
const int interruptPin = 2;

int lastRpmValue = 0;
volatile int sparkFireCount = 0;

unsigned long previousMillis = 0;
unsigned long currentMillis;

void incrementRpmCountRoutine () {
  sparkFireCount++;
}

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementRpmCountRoutine, FALLING);
  Serial.begin(9600);
}

// 4 stroke takes 2 revs for one fire
const int FIRES_PER_REV = 2;

void loop() {
  currentMillis = millis();
  if ((currentMillis - previousMillis) > LED_UPDATE_INTERVAL) {
    
    // convert sparkfire count in 200ms to one second
    int sparksPerSecond = sparkFireCount * (1000 / LED_UPDATE_INTERVAL);
    
    // convert sparks per second to spark per minute
    int sparkPerMinute = sparksPerSecond * 60;
     
    // convert spm to rpm
    int currentRpm = (sparkPerMinute) / FIRES_PER_REV;

    // average the current and last rpm for smoother results
    int averagedRpm = (currentRpm + lastRpmValue) / 2;
    
    // number of LEDs to light up based on percentage
    int percentageRPM = Math.floor((averagedRpm / MAX_RPM) * 100);
    
    // number of LEDs to light up
    int numLeds = Math.ceil(percentageRPM / (100 / TOTAL_LEDS));

    Serial.println(averagedRpm);
    
    lastRpmValue = currentRpm;

    // reset
    sparkFireCount = 0;
    previousMillis = currentMillis;
  }
}
