#include <stdint.h>
#include <Arduino.h>

#define KEY_LEFT_CTRL   0x80
#define KEY_LEFT_SHIFT  0x81
#define KEY_LEFT_ALT    0x82
#define KEY_RIGHT_CTRL  0x84
#define KEY_RIGHT_SHIFT 0x85
#define KEY_RIGHT_ALT   0x86

struct modifiers_t {
  bool ctrl;
  bool shift;
  bool alt;
};

class Keystroke {
  private:
    char _strokes[11] {};
    uint8_t _pos = 0;
    uint8_t _length = 0;
    bool _isSingle = false;
    bool _hasModifiers = false;
    modifiers_t _modifiers;
    
  public:
    Keystroke() {

    }

    Keystroke(char* strokes)
    {
        for (uint8_t i=0; i<10; i++)
        {
            if (strokes[i] == '\0')
            {
                _length = i;
                if (i == 1) {
                    _isSingle = true;
                    _length = 1;
                }
            }

            _strokes[i] = (char)strokes[i];
        }
    }

    Keystroke(char stroke)
    {
        _isSingle = true;
        _length = 1;
        _strokes[0] = stroke;
        _strokes[1] = '\0';
    }

    Keystroke setModifier(uint8_t mod, bool value) {
      switch(mod) {
        case KEY_LEFT_CTRL:
        case KEY_RIGHT_CTRL:
          _modifiers.ctrl = value;
          break;
        case KEY_LEFT_SHIFT:
        case KEY_RIGHT_SHIFT:
          _modifiers.shift = value;
          break;
        case KEY_LEFT_ALT:
        case KEY_RIGHT_ALT:
          _modifiers.alt = value;
          break;
        default:
          break;
      }
      _hasModifiers = true;
      return this;
    }

    char next()
    {
        if (_isSingle) { _pos++; return _strokes[0]; }

        char temp = _strokes[_pos++];
        if (temp == '\0') {
            // rewind();
        }

        return temp;
    }

    bool hasNext()
    {
      // Serial.println("hasNext");
      // Serial.print(_pos); Serial.print(" -> "); Serial.println(_length);
      if (_pos >= _length) {
        return false;
      }
      return true;
    }

    void rewind() {
        _pos = 0;
    }

    char* getStrokes() {
        return _strokes;
    }

    uint8_t length() {
      return _length;
    }

    bool hasModifiers() {
      return _hasModifiers;
    }

    modifiers_t getModifiers() {
      return _modifiers;
    }
};
