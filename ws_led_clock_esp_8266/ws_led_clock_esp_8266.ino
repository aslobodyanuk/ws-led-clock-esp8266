#include "constants.h"
#include "settings.h"
#include "timer2Minim.h"

#include <ESP8266WiFi.h>
#include <FileData.h>
#include <LittleFS.h>
#include <SettingsGyver.h>
#include <FastLED.h>

#define PIN 4

#define DOT_BLINK_EVERY_MS 2800
// #define MAX_DOT_BRIGHTNESS 255
#define DOT_BRIGHTNESS_STEP 3

// #define MAX_DIGIT_BRIGHTNESS 150
// #define WHOLE_STRIP_BRIGHTNESS 20

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

timerMinim _hardwareLedUpdateTimer(10);

//============ SETTINGS ============

SettingsGyver _settingsWebService("Digital Clock");

struct Config {
  uint8_t hue = 255;
  uint8_t saturation = 255;
  uint8_t digitBrightness = 150;

  uint8_t maxDotBrightness = 255;

  uint8_t hardwareBrightness = 20;
};

Config _config;
FileData _configFileSystem(&LittleFS, "/config.dat", 'A', &_config, sizeof(_config));

void apply() {
  Serial.println("Settings updated.");
  forceLedStripUpdate();
}

// ============ BUILDER ============
void build(sets::Builder& b) {
  if (b.beginGroup()) {

    b.Slider("Hue", 0, 255, 1, "", &_config.hue);
    b.Slider("Saturation", 0, 255, 1, "", &_config.saturation);
    b.Slider("Digit Brightness", 0, 255, 1, "", &_config.digitBrightness);

    b.Slider("Max Dot Brightness", 0, 255, 1, "", &_config.maxDotBrightness);

    b.Slider("Hardware Brightness", 0, 255, 1, "", &_config.hardwareBrightness);

    b.endGroup();

    if (b.wasSet()) {
      apply();
      _configFileSystem.update();
      b.reload();
      b.clearSet();
    }
  }
}

void setupGyverSettings() {
  LittleFS.begin();
  _configFileSystem.read();

  _settingsWebService.begin(true, "digital-clock");
  _settingsWebService.onBuild(build);
}

void gyverSettingsTick() {
  _settingsWebService.tick();
  _configFileSystem.tick();
}

//============ SETTINGS ============

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, PIN>(_ledStates, TOTAL_LED_COUNT).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(0);
  initializeLedState();

  setAllDigitsLoading();
  updateLedHardwareState();

  makeAccessPoint();

  // connectWifi();
  // initTime();

  // synchronizeTimeVariables();
  // setDigitTime(_hrs, _mins);

  setupGyverSettings();
  setDigitTime(18, 42);
}

void loop() {

  if (_dotBlinkTimer.isReady()) {
    _dotState = !_dotState;
  }

  effectsTick();
  timeTickerTimersTick();
  updateLedHardwareState();
  
  gyverSettingsTick();
}
