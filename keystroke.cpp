#include <stdint.h>
#include <Arduino.h>
#include <HID-Project.h>
#include "keystroke.h"


inline Keystroke* Keystroke::nextNode() { // linked list
    return _nextKeystroke;
}

inline void Keystroke::setNextNode(Keystroke *ks) {
    _nextKeystroke = ks;
}

inline bool Keystroke::hasNextNode() {
    return _nextKeystroke != NULL;
}

inline const char Keystroke::next()
{
    if (_isSingleKey) { _cursor++; return _chars[0]; }
    if (hasNext()) {
        return _chars[_cursor++];
    }
    else {
        return NULL;
    }
}

inline bool Keystroke::hasNext()
{
    return _cursor < _length;
}

inline void Keystroke::rewind() {
    _cursor = 0;
}

inline const char* Keystroke::getStrokes() {
    return _chars;
}

inline int Keystroke::getKey() {
    return _key;
}

inline uint8_t Keystroke::length() {
    return _length;
}

inline bool Keystroke::isModifier() {
    return _isModifier;
}

inline bool Keystroke::isEnum() {
    return _isEnum;
}

inline bool Keystroke::isSingleKey() {
    return _isSingleKey;
}

inline Keystroke* Keystroke::with(char key) { // single key
    Keystroke *ks = new Keystroke(key);
    append(this, ks);
    return this;
}

inline Keystroke* Keystroke::with(char* keys) { // string of keys
    Keystroke *ks = new Keystroke(keys);
    append(this, ks);
    return this;
}

inline Keystroke* Keystroke::with(int key) { // key_* enum or modifiers
    Keystroke *ks = new Keystroke(key);
    append(this, ks);
    return this;
}
