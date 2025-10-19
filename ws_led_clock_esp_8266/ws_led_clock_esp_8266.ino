#include "constants.h"
#include "settings.h"
#include "timer2Minim.h"

#include <ESP8266WiFi.h>
#include <FileData.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <FastLED.h>

#include "gyver-settings.h"

#define PIN 4

#define DOT_BLINK_EVERY_MS 2800
#define DOT_BRIGHTNESS_STEP 3

#define DEFAULT_DIGIT_BRIGHTNESS 20

CRGB _ledStates[TOTAL_LED_COUNT];

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
bool _isWifiConnected = false;

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, PIN>(_ledStates, TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(DEFAULT_DIGIT_BRIGHTNESS);
  initializeLedState();

  setupFileSystem();

  setAllDigitsLoading();
  updateLedHardwareState();

  // makeAccessPoint();

  // synchronizeTimeVariables();
  // setDigitTime(_hrs, _mins);

  // // setDigitTime(18, 42);

  // setupGyverSettings();
}

// I am calling this method in loop, because the loading gigits don`t show up correctly if called in setup
void initialize() {
  if (_isWifiConnected) {
    return;
  }
  
  updateLedHardwareState();

  connectWifi();
  initTime();

  synchronizeTimeVariables();
  setDigitTime(_hrs, _mins);

  setupGyverSettings();

  _isWifiConnected = true;
}

void loop() {
  initialize();

  if (_dotBlinkTimer.isReady()) {
    _dotState = !_dotState;
  }

  effectsTick();
  timeTickerTimersTick();
  updateLedHardwareState();

  gyverSettingsTick();
}
