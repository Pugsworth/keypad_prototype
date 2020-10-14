#include <stdint.h>
#include <Arduino.h>
#include <HID-Project.h>

/*
 * We want to model a stream of objects that represent physical key presses.
 * This makes it easy to conceptualize building a final keystroke that is executed when a button is pressed.
 * ie. Keystroke(KEY_LEFT_CTRL).with(KEY_A); // select all
 *     Keystroke(KEY_LEFT_CTRL).with(KEY_LEFT_SHIFT).with(KEY_ESC); // open task manager
 * or simply just a string of characters that are sent one after another...
 *     Keystroke("//tp @p ^ ^ ^10");
 *     
 * You sometimes might want to chain using KEY_* enums with a string.
 * ie. Keystroke(KEY_ENTER).with("Hello World!").with(KEY_ENTER); // send a chat message in many online games
 * 
 * 
 * 
 * NOTE:
 *   This could very well be modeled as a simple array of enums and strings in dynamic languages.
 *     I am using a linked list for simplicity of streaming the objects and to avoid allocating arrays at runtime.
 *     Is it the best way? Maybe...
 *   Modifier keys are implicitly released when a non-modifier key is reached.
 *     I cannot think of a scenario in which you would ever want multiple non-modifier keys held at the same time.
 *     If this is required, implementation can be changed so that all keys are held while the physical key is held,
 *       released when released respectably.
 */

/*
 * The type of keystroke this is.
 *  KEYSTROKE
 *    This is just a series of keys pressed in succession
 *   MODIFIER
 *    This is a list of modifiers held down until the modifier comes up again
 *     or the end of the entire "Keystroke" is finished
 *   COMBINATION
 *    This is a combination keystroke that incorporates held modifiers and a key
 *      ie. CTRL+SHIFT+R
 *    Could potentially just be done using MODIFIER and a single key?
 *      ie. MODIFIER(CTRL, SHIFT) + KEYSTROKE('r')
 */

enum keystroke_t {KEYSTROKE, MODIFIER, COMBINATION};

// This is both an object representing the data of a keystroke and a node
class Keystroke {
    private:
        char _chars[16] = {}; // for successive key pressed
        int _key = -1; // for KEY_* enums and modifiers
        uint8_t _cursor   = 0;
        uint8_t _length   = 0;
        bool _isSingleKey = false;
        bool _isEnum      = false; // check _key instead of _chars
        bool _isModifier  = false;
        Keystroke *_nextKeystroke = NULL;

        void append(Keystroke *parent, Keystroke *newNode) {
            Keystroke *last = parent;
            while (last->hasNextNode()) {
                last = last->nextNode();
            }

            last->setNextNode(newNode);
        }

    public:
        Keystroke() {
        }

        Keystroke(char key)
        {
            _isSingleKey = true;
            _length = 1;
            _chars[0] = key;
            _chars[1] = '\0';
        }

        Keystroke(char *keys)
        {
            for (uint8_t i=0; i<16; i++)
            {
                if (keys[i] == '\0')
                {
                    _length = i;
                    if (i == 1) {
                        _isSingleKey = true;
                        _length = 1;
                    }
                }
                _chars[i] = keys[i];
            }
        }

        Keystroke(int key) {
            _key = key;
            if (Keystroke::IsKeyModifier(key)) {
                _isModifier = true;
            }
            else {
                _isEnum = true;
            }
        }

        static bool IsKeyModifier(int key) {
            switch(key) {
                case KEY_LEFT_CTRL:
                case KEY_RIGHT_CTRL:
                case KEY_LEFT_SHIFT:
                case KEY_RIGHT_SHIFT:
                case KEY_LEFT_ALT:
                case KEY_RIGHT_ALT:
                    return true;
                    break;

                default:
                    return false;
            }
        }

        void print() {
            Serial.print(F("_key: ")); Serial.println(_key);
            Serial.print(F("*_chars: ")); Serial.println(_chars);
            Serial.flush();
        }

        Keystroke* nextNode();
        void setNextNode(Keystroke *ks);
        bool hasNextNode();
        const char next();
        bool hasNext();
        void rewind();
        const char* getStrokes();
        int getKey();
        uint8_t length();
        bool isModifier();
        bool isEnum();
        bool isSingleKey();

        Keystroke* with(char key);
        Keystroke* with(char* keys);
        Keystroke* with(int key);
};
