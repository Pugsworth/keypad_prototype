#include <Arduino.h>

class Sliders {
  private:
    int _numSliders = 0;
    int _sliderInputs[6] = {};
    int _sliderValues[6] = {};
    
  public:
    Sliders() {
    
    }

    Sliders(int numSliders, int inputs[]) {
      _numSliders = numSliders;

      for (int i = 0; i < _numSliders; i++) {
        _sliderInputs[i] = inputs[i];
      }

      delete inputs;
    }

    void begin() {
      for (int i = 0; i < _numSliders; i++) {
        pinMode(_sliderInputs[i], INPUT);
      }
    }

    void update() {
      for (int i = 0; i < _numSliders; i++) {
        _sliderValues[i] = analogRead(_sliderInputs[i]);
      }
    }

    void sendValues() {
      String buildString = String("");

      for (int i = 0; i < _numSliders; i++) {
        buildString += String((int)_sliderValues[i]);

        if (i < _numSliders - 1) {
          buildString += String("|");
        }
      }

      Serial.println(buildString);
    }
  
};
