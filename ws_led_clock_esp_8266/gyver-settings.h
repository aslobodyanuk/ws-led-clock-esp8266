//============ SETTINGS ============
SettingsGyver _settingsWebService("Digital Clock");

struct Config {
  uint8_t hue = 0;
  uint8_t saturation = 255;
  uint8_t digitBrightness = 150;

  uint8_t maxDotBrightness = 255;

  uint8_t hardwareBrightness = 20;
};

Config _config;
FileData _configFileSystem(&LittleFS, "/config.dat", 'A', &_config, sizeof(_config));

void forceLedStripUpdate();

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

void setupFileSystem() {
  LittleFS.begin();
  _configFileSystem.read();
}

void setupGyverSettings() {
  _settingsWebService.begin(true, "digital-clock");
  _settingsWebService.onBuild(build);
}

void gyverSettingsTick() {
  _settingsWebService.tick();
  _configFileSystem.tick();
}

//============ SETTINGS ============