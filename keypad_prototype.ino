#include <Keypad.h>
#include <HID-Project.h>
#include "keystroke.cpp"
#include "Sliders.cpp"

static const int pinLED = 4;

static const int rows = 3;
static const int cols = 3;

// Key ID matrix. Locations are rotated by library for some reason.
char keys[rows][cols] = {
    {6, 3, 0},
    {7, 4, 1},
    {8, 5, 2}
};

/* ID to keystroke array.
 * Rotation corrected
 * |0|1|2|
 * |3|4|5|
 * |6|7|8|
 */
Keystroke *keystrokes[] {
    new Keystroke("Hello World!"),
    (new Keystroke(KEY_LEFT_SHIFT))->with('t'),
    new Keystroke('t'),

    new Keystroke(KEY_LEFT_ARROW),
    new Keystroke(KEYPAD_5),
    new Keystroke(KEY_RIGHT_ARROW),

    new Keystroke(KEY_A),
    new Keystroke('9'),
    (new Keystroke(KEY_LEFT_CTRL))->with(KEY_LEFT_SHIFT)->with(KEY_ESC)
};

byte colPins[cols] = {10, 16, 14};
byte rowPins[rows] = {7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
Sliders sliders = Sliders(2, (const uint8_t[]){A3, A2});

long slidersNextUpdate = millis();
long slidersUpdateRate = 10; // 100ms

long lightNextUpdate = millis();
long lightUpdateRate = 50; // 100ms

void setup()
{
    pinMode(pinLED, OUTPUT);
    Serial.begin(9600);
    sliders.begin();
    BootKeyboard.begin();
    while (!Serial){}

    keypad.setDebounceTime(5);
    keypad.addEventListener(keypadEvent);
}

void checkNumLock() {
    if (!(BootKeyboard.getLeds() & LED_NUM_LOCK)) {
        BootKeyboard.write(KEY_NUM_LOCK);
    }
}

void sendKeystroke(KeypadEvent btn)
{
    Keystroke *node = keystrokes[btn];
    bool used_modifier = false;

    while(node != NULL) {
        light();

        if (node->isModifier()) {
            BootKeyboard.press((KeyboardKeycode)node->getKey());
            used_modifier = true;
        }
        else if (node->isEnum()) {
            BootKeyboard.write((KeyboardKeycode)node->getKey());
        }
        else if (node->isSingleKey()) {
            BootKeyboard.write(node->next());
        }
        else {
            node->rewind();
            while (node->hasNext()) {
                BootKeyboard.write(node->next());
            }
        }

        node = node->nextNode();
    }

    if (used_modifier) {
        BootKeyboard.releaseAll();
    }

}

void keypadEvent(KeypadEvent key) {
    KeyState state = keypad.getState();
    switch(state) {
        case IDLE:
            break;
        case PRESSED:
            sendKeystroke(key);
            break;
        case HOLD:
            break;
        case RELEASED:
            break;
        default:
            break;
    }
}

void loop()
{
    // sliders update
    if (slidersNextUpdate < millis()) {
        sliders.update();
        sliders.sendValues();
        slidersNextUpdate = millis() + slidersUpdateRate;
    }

    // light update
    if (lightNextUpdate < millis()) {
        digitalWrite(pinLED, LOW);
    }

    checkNumLock(); // ensure numlock is always enabled

    keypad.getKeys();
}

void light() {
    digitalWrite(pinLED, HIGH);
    lightNextUpdate = millis() + lightUpdateRate;
}
