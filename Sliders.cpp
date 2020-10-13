#include <Arduino.h>

const unsigned int SLIDER_DIFFERENCE_DIRTY = 3;

typedef struct slider_t {
  uint8_t pin;
  unsigned int value;
  bool dirtyFlag;

  slider_t(){};
} Slider;

class Sliders {
  private:
    uint8_t _numSliders = 0;
    Slider* _sliders[6] = {};
    
  public:
    Sliders() {
    
    }

    Sliders(uint8_t numSliders, const uint8_t inputs[]) {
      _numSliders = numSliders;

      for (uint8_t i = 0; i < _numSliders; i++) {
        _sliders[i] = new Slider();
        _sliders[i]->pin = inputs[i];
      }
    }

    void begin() {
      for (uint8_t i = 0; i < _numSliders; i++) {
        pinMode(_sliders[i]->pin, INPUT);
      }
    }

    void update() {
      for (uint8_t i = 0; i < _numSliders; i++) {
        unsigned int oldValue = _sliders[i]->value;
        unsigned int newValue = analogRead(_sliders[i]->pin);
        
        if (abs(oldValue - newValue) > SLIDER_DIFFERENCE_DIRTY) {
          _sliders[i]->value = newValue;
          _sliders[i]->dirtyFlag = true;
        }
      }
    }

    void sendValues() {
      String buildString = String("");

      bool isDirty = false;

      // first, we have to check if anything changed
      for (uint8_t i = 0; i < _numSliders; i++) {
        if (_sliders[i]->dirtyFlag) {
          isDirty = true;
          break;
        }
      }

      if (isDirty) {
        for (uint8_t i = 0; i < _numSliders; i++) {
          buildString += String((int)_sliders[i]->value);
          _sliders[i]->dirtyFlag = false;
  
          if (i < _numSliders - 1) {
            buildString += String("|");
          }
        }
  
        Serial.println(buildString);
      }
    }
};
