/*
 * SIMPLE ARDUINO BIKE TACHOMETER
 */

const uint8_t TOTAL_LEDS = 24;
const uint8_t interruptPin = 2;

const int LED_UPDATE_INTERVAL = 100;
const int MAX_RPM = 2000;
const int FIRES_PER_REV = 2;
int lastRpmValue = 0;

unsigned long lastInterrupt;
unsigned long previousMillis = 0;
unsigned long currentMillis;

volatile int sparkFireCount = 0;

void incrementRpmCountRoutine () {
  if(millis() - lastInterrupt >= 20) {    
    sparkFireCount++;
    lastInterrupt = millis();
  }
}

void setup() {
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), incrementRpmCountRoutine, FALLING);
  Serial.begin(9600);
}


void loop(){
  rpmLEDloop();
}

void rpmLEDloop() {
  currentMillis = millis();
  if ((currentMillis - previousMillis) >= LED_UPDATE_INTERVAL) {

    detachInterrupt(digitalPinToInterrupt(interruptPin)); //Disable interrupt when calculating

    // convert sparkfire count in 200ms to one second
    int sparksPerSecond = sparkFireCount * (1000 / LED_UPDATE_INTERVAL);
    
    // convert sparks per second to spark per minute
    int sparkPerMinute = sparksPerSecond * 60;
     
    // convert spm to rpm
    int currentRpm = (sparkPerMinute) / FIRES_PER_REV;

    // average the current and last rpm for smoother results
    int averagedRpm = (currentRpm + lastRpmValue) / 2;

    Serial.print(F("averagedRpm = "));
    Serial.println(averagedRpm);
    
    // number of LEDs to light up based on percentage
    float percentageRPM = ((float) averagedRpm /  (float) MAX_RPM) * 100.0;
    
    Serial.print(F("percentageRPM = "));
    Serial.println(percentageRPM);
    
    // number of LEDs to light up
    int numLeds = ceil(percentageRPM / (100 / TOTAL_LEDS));

    Serial.print(F("numLeds = "));
    Serial.println(numLeds);
    

    // reset
    sparkFireCount = 0;
    lastRpmValue = currentRpm;
    previousMillis = currentMillis;
    attachInterrupt(digitalPinToInterrupt(interruptPin), incrementRpmCountRoutine, FALLING);
  }
}
