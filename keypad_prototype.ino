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
  new Keystroke(KEY_LEFT_SHIFT),
  (new Keystroke(KEY_LEFT_SHIFT))->with('t'),
  new Keystroke('t'),
  
  new Keystroke(KEY_LEFT_ARROW),
  new Keystroke("Testing!"),
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
  Keyboard.begin();
  while (!Serial){}

  Serial.print("'9' is "); Serial.println('9');
  Keystroke ks = Keystroke('9');
  Serial.print("Keystroke('9') is "); Serial.println((char)ks.next());

  Serial.print("KEY_A is "); Serial.println(KEY_A);
  ks = Keystroke(KEY_A);
  Serial.print("Keystroke(KEY_A) is "); Serial.println(ks.getKey());

  Serial.flush();
  

  keypad.setDebounceTime(5);
  keypad.addEventListener(keypadEvent);
}

void sendKeystroke(KeypadEvent btn)
{
  Keystroke *node = keystrokes[btn];
  bool used_modifier = false;
  
  while(node != NULL) {
    light();

    node->print();

    if (node->isModifier()) {
      Serial.print("Modifier is being pressed: "); Serial.println(node->getKey());
      Keyboard.press((KeyboardKeycode)node->getKey());
      used_modifier = true;
    }
    else if (node->isEnum()) {
      Keyboard.write((KeyboardKeycode)node->getKey());
    }
    else if (node->isSingleKey()) {
      Keyboard.write(node->next());
    }
    else {
      node->rewind();
      while (node->hasNext()) {
        const char _ch = node->next();
        Keyboard.write(_ch);
      }
    }
    
    node = node->nextNode();
    Serial.println();
  }

  if (used_modifier) {
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
