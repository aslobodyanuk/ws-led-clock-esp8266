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

// Mapping for the led indexes for each digit
int _digitLedsMapping[TOTAL_NUMBERS_COUNT][2] = {
  { 0, DIGIT_TOTAL_LEDS },
  { DIGIT_TOTAL_LEDS, DIGIT_TOTAL_LEDS * 2 },
  { DIGIT_TOTAL_LEDS * 2 + DOT_LED_COUNT, DIGIT_TOTAL_LEDS * 3 + DOT_LED_COUNT },
  { DIGIT_TOTAL_LEDS * 3 + DOT_LED_COUNT, DIGIT_TOTAL_LEDS * 4 + DOT_LED_COUNT }
};

// Mapping of led indexes for the dots
int _dotLedsMapping[2] = { DIGIT_TOTAL_LEDS * 2, DIGIT_TOTAL_LEDS * 2 + DOT_LED_COUNT };


void setDigitValueWithAnimation(int16_t value) {
  int16_t minutes = value % 100;
  int16_t hours = value / 100;

  setDigitTimeWithAnimation(abs(hours), abs(minutes));
}

void setDigitTimeWithAnimation(byte hours, byte minutes) {
  _newDigits[0] = (byte)hours / 10;
  _newDigits[1] = (byte)hours % 10;

  _newDigits[2] = (byte)minutes / 10;
  _newDigits[3] = (byte)minutes % 10;

  _animateDigit[0] = (_newDigits[0] != _digits[0]);
  _animateDigit[1] = (_newDigits[1] != _digits[1]);
  _animateDigit[2] = (_newDigits[2] != _digits[2]);
  _animateDigit[3] = (_newDigits[3] != _digits[3]);

  _digitAnimationService.setState(false);
}

void updateLedStateForAllDigits() {
  int currentAnimationBrightness = _digitAnimationService.getValue();

  byte brightnessForDigits[4] = {
    _animateDigit[0] ? currentAnimationBrightness : _config.digitBrightness,
    _animateDigit[1] ? currentAnimationBrightness : _config.digitBrightness,
    _animateDigit[2] ? currentAnimationBrightness : _config.digitBrightness,
    _animateDigit[3] ? currentAnimationBrightness : _config.digitBrightness
  };

  updateLedStateForDigit(_digitLedsMapping[0][0], _digitLedsMapping[0][1], _digits[0], brightnessForDigits[0]);
  updateLedStateForDigit(_digitLedsMapping[1][0], _digitLedsMapping[1][1], _digits[1], brightnessForDigits[1]);
  updateLedStateForDigit(_digitLedsMapping[2][0], _digitLedsMapping[2][1], _digits[2], brightnessForDigits[2]);
  updateLedStateForDigit(_digitLedsMapping[3][0], _digitLedsMapping[3][1], _digits[3], brightnessForDigits[3]);
}

void updateLedStateForDigit(int start, int end, byte digitNumber, byte brightness) {
  fillLedSegment(start, end, CRGB::Black);

  if (digitNumber > 9) return;  // only digits 0–9 supported

  CHSV letStripColour = getLedStripColour(brightness);

  // For each of 7 segments
  for (int seg = 0; seg < SEGMENT_COUNT; seg++) {
    if (_segmentMap[digitNumber][seg]) {
      int segStart = start + seg * SEGMENT_SIZE;
      int segEnd = segStart + SEGMENT_SIZE;

      fillLedSegment(segStart, segEnd, letStripColour);
    }
  }
}

CHSV getLedStripColour(byte brightness) {
  return CHSV(_config.hue, _config.saturation, brightness);
}

void updateLedStateForDots() {
  int currentAnimationBrightness = _dotAnimationService.getValue();
  fillLedSegment(_dotLedsMapping[0], _dotLedsMapping[1], CHSV(_config.hue, _config.saturation, currentAnimationBrightness));
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
  _digits[0] = (byte)hours / 10;
  _digits[1] = (byte)hours % 10;

  _digits[2] = (byte)minutes / 10;
  _digits[3] = (byte)minutes % 10;

  _newDigits[0] = _digits[0];
  _newDigits[1] = _digits[1];
  _newDigits[2] = _digits[2];
  _newDigits[3] = _digits[3];

  updateLedStateForAllDigits();
}

void setDigitValue(int16_t value) {
  int16_t minutes = value % 100;
  int16_t hours = value / 100;

  setDigitTime(abs(hours), abs(minutes));
}