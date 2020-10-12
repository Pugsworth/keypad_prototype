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

// ID to keystroke array.
// Rotation corrected
// |0|1|2|
// |3|4|5|
// |6|7|8|
Keystroke keystrokes[] {
  Keystroke('t').setModifier(KEY_LEFT_SHIFT, true),
  Keystroke('b'),
  Keystroke('c'),
  
  Keystroke('d'),
  Keystroke('e'),
  Keystroke('f'),
  
  Keystroke('g'),
  Keystroke('h'),
  Keystroke('t')
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
  Keyboard.begin();
  delay(1000);

  keypad.setDebounceTime(5);

  keypad.addEventListener(keypadEvent);

  // char test[] = {KEY_LEFT_CTRL, KEY_BACKSPACE, '\0'};
  // keystrokes[8] = Keystroke(test);
}

void sendKeystroke(KeypadEvent key)
{
  // Keyboard.write((KeyboardKeycode) key);
  Keystroke strokes = keystrokes[(uint8_t)key];

  // Serial.print((uint8_t)key); Serial.print(" - "); Serial.println((char[]) strokes.getStrokes());

  bool has_modifiers = strokes.hasModifiers();

  if (has_modifiers) {
    modifiers_t modifiers = strokes.getModifiers();
    
    if (modifiers.ctrl) {
      Keyboard.press(KEY_LEFT_CTRL);
    }
    if (modifiers.shift) {
      Keyboard.press(KEY_LEFT_SHIFT);
    }
    if (modifiers.alt) {
      Keyboard.press(KEY_LEFT_ALT);
    }
  }
  
  while (strokes.hasNext())
  {
    uint8_t nextStroke = strokes.next();
    char stroke = (char)nextStroke;

    light();

      Keyboard.write(stroke);
  }
  
  if (has_modifiers) {
    Keyboard.releaseAll();
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
  
  keypad.getKeys();
}

void light() {
  digitalWrite(pinLED, HIGH);
  lightNextUpdate = millis() + lightUpdateRate;
}
