#include "constants.h"
#include "settings.h"
#include "timer2Minim.h"
#include "animationService.h"

#include <ESP8266WiFi.h>
#include <FileData.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <FastLED.h>

#include "gyver-settings.h"

//#define DEBUG_MODE 1                                              // Creates it`s own WIFI point and simulates time passing by, comment out to disable

#define PIN 4

#define DOT_BLINK_EVERY_MS 2800
#define DOT_BRIGHTNESS_STEP 3
#define DIGIT_BRIGHTNESS_STEP 3

#define DEFAULT_DIGIT_BRIGHTNESS 20

CRGB _ledStates[TOTAL_LED_COUNT];

byte _digits[4];                                                  // Current digit number values 0-9
byte _newDigits[4];                                               // Next digit number values 0-9 (during animation stores next value)
bool _animateDigit[4] = { false, false, false, false };           // Stores values whether digit should be animated (at runtime gets calculated)

timerMinim _dotBlinkTimer(DOT_BLINK_EVERY_MS);
timerMinim _effectsTimer(10);

animationService _digitAnimationService(255, DIGIT_BRIGHTNESS_STEP);
animationService _dotAnimationService(255, DOT_BRIGHTNESS_STEP);

#define SYNC_TIME_EVERY_MINUTES 10
timerMinim _syncTimeTimer(1000 * 60 * SYNC_TIME_EVERY_MINUTES);
timerMinim _everySecondTimer(1000);

int _hrs, _mins, _secs;
bool _isWifiConnected = false;

#ifndef DEBUG_MODE
void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, PIN>(_ledStates, TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(DEFAULT_DIGIT_BRIGHTNESS);
  initializeLedState();

  setupFileSystem();

  updateAnimationServicesConfiguration();
  setAllDigitsLoading();
  updateLedHardwareState();
}

// I am calling this method in loop, because the loading digits don`t show up correctly if called in setup
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
    _dotAnimationService.setState(true);
  }

  effectsTick();
  timeTickerTimersTick();
  updateLedHardwareState();

  gyverSettingsTick();
}
#endif

//============ DEBUG_MODE ============

#ifdef DEBUG_MODE

timerMinim _secondEverySecondTimer(3000);

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, PIN>(_ledStates, TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(DEFAULT_DIGIT_BRIGHTNESS);
  initializeLedState();

  setupFileSystem();

  updateAnimationServicesConfiguration();
  setAllDigitsLoading();
  updateLedHardwareState();

  makeAccessPoint();

  setDigitTime(_hrs, _mins);

  setupGyverSettings();
}

void loop() {

  if (_secondEverySecondTimer.isReady()) {
    _secs = 59;
  }

  if (_dotBlinkTimer.isReady()) {
    _dotAnimationService.setState(true);
  }

  effectsTick();
  timeTickerTimersTick();
  updateLedHardwareState();

  gyverSettingsTick();
}
#endif

//============ DEBUG_MODE ============