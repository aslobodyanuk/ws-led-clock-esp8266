
class animationService
{
  public:
    animationService(int maxValue, int step);
    void setStep(int step);
    void setMaxValue(int step);
    void setState(bool state);
    bool getState();
    int getValue();
    int tick();

  private:
    bool _state = false;
    int _currentValue = 0;
    int _maxValue = 0;
    int _step = 0;
};

animationService::animationService(int maxValue, int step) {
  _maxValue = maxValue;
  _step = step;
}

void animationService::setStep(int step) {
  _step = step;
}

void animationService::setMaxValue(int maxValue) {
  _maxValue = maxValue;
}

void animationService::setState(bool state) {
  _state = state;
}

bool animationService::getState() {
  return _state;
}

int animationService::getValue() {
  return _currentValue;
}

int animationService::tick() {
  if (_state == false && _currentValue > 0) {
    if (_currentValue - _step < 0) {
      _currentValue = 0;
    } else {
      _currentValue -= _step;
    }
  }

  if (_state == true && _currentValue < _maxValue) {
    if (_currentValue + _step > _maxValue) {
      _currentValue = _maxValue;
    } else {
      _currentValue += _step;
    }
  }

  return _currentValue;
}
