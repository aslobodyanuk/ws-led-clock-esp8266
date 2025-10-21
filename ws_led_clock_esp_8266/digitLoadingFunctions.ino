bool _loadingMapping[SEGMENT_COUNT] = { 1, 0, 0, 0, 0, 0, 0 };

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