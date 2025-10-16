
#define SEGMENT_COUNT 7
#define SEGMENT_SIZE 6

#define TOTAL_NUMBERS_COUNT 4

#define DOT_LED_COUNT 2

#define DIGIT_TOTAL_LEDS SEGMENT_SIZE* SEGMENT_COUNT
#define TOTAL_LED_COUNT DIGIT_TOTAL_LEDS* TOTAL_NUMBERS_COUNT + DOT_LED_COUNT

uint32_t _ledStates[TOTAL_LED_COUNT];

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
  fillLedSegment(_dotLedsMapping[0], _dotLedsMapping[1], _pixels.Color(_currentDotBrightness, 0, 0));
}

void initializeLedState() {
  for (int counter = 0; counter < TOTAL_LED_COUNT; counter++) {
    _ledStates[counter] = _pixels.Color(0, 0, 0);
  }
}

void updateLedHardwareState() {
  _pixels.clear();
  for (int i = 0; i < TOTAL_LED_COUNT; i++) {
    _pixels.setPixelColor(i, _ledStates[i]);
  }
  _pixels.show();
}

void updateLedStateForDigit(int start, int end, byte value) {
  fillLedSegment(start, end, _pixels.Color(0, 0, 0));

  if (value > 9) return;  // only digits 0–9 supported

  // For each of 7 segments
  for (int seg = 0; seg < SEGMENT_COUNT; seg++) {
    if (_segmentMap[value][seg]) {
      int segStart = start + seg * SEGMENT_SIZE;
      int segEnd = segStart + SEGMENT_SIZE;

      fillLedSegment(segStart, segEnd, _pixels.Color(MAX_DIGIT_BRIGHTNESS, 0, 0));
    }
  }
}

// Fill the LEDs for this segment with certain color
void fillLedSegment(int start, int end, uint32_t value) {
  for (int i = start; i < end; i++) {
    _ledStates[i] = value;
  }
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
  fillLedSegment(start, end, _pixels.Color(0, 0, 0));

  // For each of 7 segments
  for (int seg = 0; seg < SEGMENT_COUNT; seg++) {
    if (_loadingMapping[seg]) {
      int segStart = start + seg * SEGMENT_SIZE;
      int segEnd = segStart + SEGMENT_SIZE;

      fillLedSegment(segStart, segEnd, _pixels.Color(MAX_DIGIT_BRIGHTNESS, 0, 0));
    }
  }
}