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
  Keystroke(KEY_LEFT_SHIFT).with('t'),
  Keystroke(KEYPAD_8),
  Keystroke(KEYPAD_9),
  
  Keystroke(KEY_LEFT_ARROW),
  Keystroke(KEY_MUTE),
  Keystroke(KEY_RIGHT_ARROW),
  
  Keystroke(KEY_6),
  Keystroke('9'),
  Keystroke(KEY_LEFT_CTRL).with(KEY_LEFT_SHIFT).with(KEY_ESC)
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
  while (!Serial){}
  delay(1000);

  Keystroke k = Keystroke(KEY_LEFT_SHIFT).with('t');
  // Serial.println(k->getStrokes());

  keypad.setDebounceTime(5);
  keypad.addEventListener(keypadEvent);
}

void sendKeystroke(KeypadEvent btn)
{
  // Keyboard.write((KeyboardKeycode) key);
  Keystroke *node = &keystrokes[btn];

  // Serial.print((uint8_t)key); Serial.print(" - "); Serial.println((char[]) strokes.getStrokes());

  bool used_modifier = false;
  
  while(node != NULL) {
    light();

    if (node->isModifier()) {
      Keyboard.press(node->getKey());
      used_modifier = true;
    }
    else if (node->isEnum()) {
      node->print();
      Keyboard.write(node->getKey());
    }
    else if (node->isSingleKey()) {
      Keyboard.write(node->next());
    }
    else {
      Serial.println(F("writing characters:"));
      
      node->rewind();
      while (node->hasNext()) {
        const char _ch = node->next();
        Keyboard.write(_ch);
        Serial.print(_ch);
      }

      Serial.println();
    }

    if (used_modifier) {
      Keyboard.releaseAll();
    }
    
    node = node->nextNode();
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
    // sliders.sendValues();
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
