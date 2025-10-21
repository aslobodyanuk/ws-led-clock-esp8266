void timeTickerTimersTick() {
  if (_syncTimeTimer.isReady()) {
    synchronizeTimeVariables();
  }

  if (!_everySecondTimer.isReady()) {
    return;
  }

  timeTick();
}

void timeTick() {
  bool hasChanged = false;
  _secs++;

  if (_secs > 59) {
    _secs = 0;
    _mins++;
    hasChanged = true;
  }

  if (_mins > 59) {
    _mins = 0;
    _hrs++;

    if (_hrs > 23) _hrs = 0;

    hasChanged = true;
  }

  if (hasChanged) {
    setDigitTimeWithAnimation(_hrs, _mins);
  }
}

void synchronizeTimeVariables() {
  time_t now = time(nullptr);
  struct tm * timeinfo = localtime(&now);

  _hrs = timeinfo->tm_hour;
  _mins = timeinfo->tm_min;
  _secs = timeinfo->tm_sec;

  Serial.printf("Synchronized time variables: %02d:%02d:%02d\n", _hrs, _mins, _secs);
}