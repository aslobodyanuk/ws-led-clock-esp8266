void effectsTick() {
  if (!_effectsTimer.isReady()) {
    return;
  }

  dotEffectsTick();
  digitEffectsTick();
}

void dotEffectsTick() {
  int currentBrightness = _dotAnimationService.tick();

  if (currentBrightness == _config.maxDotBrightness) {
    handleDotAnimationComplete();
  }

  updateLedStateForDots();
}

void handleDotAnimationComplete() {
  _dotAnimationService.setState(!_dotAnimationService.getState());
}

void digitEffectsTick() {
  int currentBrightness = _digitAnimationService.tick();

  if (currentBrightness == 0) {
    handleDigitFadeOutComplete();
  }

  updateLedStateForAllDigits();
}

void handleDigitFadeOutComplete() {
  _digits[0] = _newDigits[0];
  _digits[1] = _newDigits[1];
  _digits[2] = _newDigits[2];
  _digits[3] = _newDigits[3];

  _digitAnimationService.setState(true);
}

void updateAnimationServicesConfiguration() {
  _digitAnimationService.setMaxValue(_config.digitBrightness);
  _dotAnimationService.setMaxValue(_config.maxDotBrightness);
}