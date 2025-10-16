void effectsTick() {
  if (!_effectsTimer.isReady()) {
    return;
  }

  dotEffectsTick();
}

void dotEffectsTick() {
  if (_dotState == false && _currentDotBrightness > 0) {
    if (_currentDotBrightness - DOT_BRIGHTNESS_STEP < 0) {
      _currentDotBrightness = 0;
    } else {
      _currentDotBrightness -= DOT_BRIGHTNESS_STEP;
    }
  }

  if (_dotState == true && _currentDotBrightness < MAX_DOT_BRIGHTNESS) {
    if (_currentDotBrightness + DOT_BRIGHTNESS_STEP > MAX_DOT_BRIGHTNESS) {
      _currentDotBrightness = MAX_DOT_BRIGHTNESS;
    } else {
      _currentDotBrightness += DOT_BRIGHTNESS_STEP;
    }

    if (_currentDotBrightness == MAX_DOT_BRIGHTNESS) {
      _dotState = false;
    }
  }

  setDotValue();
}