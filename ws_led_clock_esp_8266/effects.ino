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

  if (_dotState == true && _currentDotBrightness < _config.maxDotBrightness) {
    if (_currentDotBrightness + DOT_BRIGHTNESS_STEP > _config.maxDotBrightness) {
      _currentDotBrightness = _config.maxDotBrightness;
    } else {
      _currentDotBrightness += DOT_BRIGHTNESS_STEP;
    }

    if (_currentDotBrightness == _config.maxDotBrightness) {
      _dotState = false;
    }
  }

  setDotValue();
}