// ---------------- Segment Map ----------------
// My setup
//  --D--
// |     |
// C     E
// |     |
//  --F--
// |     |
// B     G
// |     |
//  --A--

bool _segmentMap[10][SEGMENT_COUNT] = {
  // A, B, C, D, E, F, G
  { 1, 1, 1, 1, 1, 0, 1 },  // 0
  { 0, 0, 0, 0, 1, 0, 1 },  // 1
  { 1, 1, 0, 1, 1, 1, 0 },  // 2
  { 1, 0, 0, 1, 1, 1, 1 },  // 3
  { 0, 0, 1, 0, 1, 1, 1 },  // 4
  { 1, 0, 1, 1, 0, 1, 1 },  // 5
  { 1, 1, 1, 1, 0, 1, 1 },  // 6
  { 0, 0, 0, 1, 1, 0, 1 },  // 7
  { 1, 1, 1, 1, 1, 1, 1 },  // 8
  { 1, 0, 1, 1, 1, 1, 1 }   // 9
};

bool _loadingMapping[SEGMENT_COUNT] = { 1, 0, 0, 0, 0, 0, 0 };

int _digitLedsMapping[TOTAL_NUMBERS_COUNT][2] = {
  { 0, DIGIT_TOTAL_LEDS },
  { DIGIT_TOTAL_LEDS, DIGIT_TOTAL_LEDS * 2 },
  { DIGIT_TOTAL_LEDS * 2 + DOT_LED_COUNT, DIGIT_TOTAL_LEDS * 3 + DOT_LED_COUNT },
  { DIGIT_TOTAL_LEDS * 3 + DOT_LED_COUNT, DIGIT_TOTAL_LEDS * 4 + DOT_LED_COUNT }
};

int _dotLedsMapping[2] = { DIGIT_TOTAL_LEDS * 2, DIGIT_TOTAL_LEDS * 2 + DOT_LED_COUNT };

void test(int value) {
  // updateLedStateForDigit(_digitLedsMapping[0][0], _digitLedsMapping[0][1], value);
  // updateLedStateForDigit(_digitLedsMapping[1][0], _digitLedsMapping[1][1], value);
  // updateLedStateForDigit(_digitLedsMapping[2][0], _digitLedsMapping[2][1], value);
  updateLedStateForDigit(_digitLedsMapping[3][0], _digitLedsMapping[3][1], value);
}

void setDotValue() {
  // fillLedSegment(_dotLedsMapping[0], _dotLedsMapping[1], _pixels.Color(_currentDotBrightness, 0, 0));
  fillLedSegment(_dotLedsMapping[0], _dotLedsMapping[1], CHSV(_config.hue, _config.saturation, _currentDotBrightness));
}

void initializeLedState() {
  for (int counter = 0; counter < TOTAL_LED_COUNT; counter++) {
    _ledStates[counter] = CRGB::Black;
  }
}

void updateLedHardwareState() {
  FastLED.setBrightness(_config.hardwareBrightness);
  FastLED.show();
}

void updateLedStateForDigit(int start, int end, byte value) {
  fillLedSegment(start, end, CRGB::Black);

  if (value > 9) return;  // only digits 0–9 supported

  // For each of 7 segments
  for (int seg = 0; seg < SEGMENT_COUNT; seg++) {
    if (_segmentMap[value][seg]) {
      int segStart = start + seg * SEGMENT_SIZE;
      int segEnd = segStart + SEGMENT_SIZE;

      fillLedSegment(segStart, segEnd, getLedStripColour());
    }
  }
}

CHSV getLedStripColour() {
  return CHSV(_config.hue, _config.saturation, _config.digitBrightness);
}

// Fill the LEDs for this segment with certain color
void fillLedSegment(int start, int end, CRGB::HTMLColorCode value) {
  for (int i = start; i < end; i++) {
    _ledStates[i] = value;
  }
}

void fillLedSegment(int start, int end, CHSV value) {
  for (int i = start; i < end; i++) {
    _ledStates[i] = value;
  }
}

void forceLedStripUpdate() {
  setDigitTime(_hrs, _mins);
}

void setDigitTime(byte hours, byte minutes) {
  indiDigits[0] = (byte)hours / 10;
  indiDigits[1] = (byte)hours % 10;

  indiDigits[2] = (byte)minutes / 10;
  indiDigits[3] = (byte)minutes % 10;

  updateLedStateForDigit(_digitLedsMapping[0][0], _digitLedsMapping[0][1], indiDigits[0]);
  updateLedStateForDigit(_digitLedsMapping[1][0], _digitLedsMapping[1][1], indiDigits[1]);
  updateLedStateForDigit(_digitLedsMapping[2][0], _digitLedsMapping[2][1], indiDigits[2]);
  updateLedStateForDigit(_digitLedsMapping[3][0], _digitLedsMapping[3][1], indiDigits[3]);
}

// // for effects
// void setNewTime() {
//   newTime[0] = (byte)hrs / 10;
//   newTime[1] = (byte)hrs % 10;

//   newTime[2] = (byte)mins / 10;
//   newTime[3] = (byte)mins % 10;
// }

void setDigitValue(int16_t value) {
  int16_t minutes = value % 100;
  int16_t hours = value / 100;

  setDigitTime(abs(hours), abs(minutes));
}

void setAllDigitsLoading() {
  setDigitLoading(_digitLedsMapping[0][0], _digitLedsMapping[0][1]);
  setDigitLoading(_digitLedsMapping[1][0], _digitLedsMapping[1][1]);
  setDigitLoading(_digitLedsMapping[2][0], _digitLedsMapping[2][1]);
  setDigitLoading(_digitLedsMapping[3][0], _digitLedsMapping[3][1]);
}

void setDigitLoading(int start, int end) {
  fillLedSegment(start, end, CRGB::Black);

  // For each of 7 segments
  for (int seg = 0; seg < SEGMENT_COUNT; seg++) {
    if (_loadingMapping[seg]) {
      int segStart = start + seg * SEGMENT_SIZE;
      int segEnd = segStart + SEGMENT_SIZE;

      fillLedSegment(segStart, segEnd, CRGB::Red);
    }
  }
}