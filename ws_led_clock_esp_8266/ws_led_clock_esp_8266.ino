#include "settings.h"
#include <ESP8266WiFi.h>

#include <Adafruit_NeoPixel.h>
#include "timer2Minim.h"

#define PIN 4

#define NUMPIXELS 170
#define DELAYVAL 10

#define DOT_BLINK_EVERY_MS 2800
#define MAX_DOT_BRIGHTNESS 255
#define DOT_BRIGHTNESS_STEP 3

#define MAX_DIGIT_BRIGHTNESS 150
#define WHOLE_STRIP_BRIGHTNESS 20

Adafruit_NeoPixel _pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

volatile byte indiDigits[4];

// Dot
volatile bool _dotState = false;
volatile byte _currentDotBrightness = 0;

timerMinim _dotBlinkTimer(DOT_BLINK_EVERY_MS);
timerMinim _effectsTimer(10);
// Dot

#define SYNC_TIME_EVERY_MINUTES 10
timerMinim _syncTimeTimer(1000 * 60 * SYNC_TIME_EVERY_MINUTES);
timerMinim _everySecondTimer(1000);

int _hrs, _mins, _secs;

void setup() {
  Serial.begin(115200);

  _pixels.begin();
  _pixels.setBrightness(WHOLE_STRIP_BRIGHTNESS);
  initializeLedState();

  setAllDigitsLoading();
  updateLedHardwareState();

  connectWifi();
  initTime();

  synchronizeTimeVariables();
  setDigitTime(_hrs, _mins);

  setDigitTime(18, 42);
}

void loop() {

  if (_dotBlinkTimer.isReady()) {
    _dotState = !_dotState;
  }

  effectsTick();
  timeTickerTimersTick();
  updateLedHardwareState();
}
