// #include <Button.h>

static const int pinLED = 4;

static const int rows = 3;
static const int cols = 3;

char keys[rows][cols] = {
  {'a', 'b', 'c'},
  {'d', 'e', 'f'},
  {'h', 'i', 'j'}
};

byte colPins[cols] = {7, 8, 9};
byte rowPins[rows] = {14, 16, 10};

void setup() {
  pinMode(pinLED, OUTPUT);

  // rows
  pinMode(colPins[0], INPUT_PULLUP);
  pinMode(colPins[1], INPUT_PULLUP);
  pinMode(colPins[2], INPUT_PULLUP);

  //cols
  pinMode(rowPins[0], OUTPUT);
  pinMode(rowPins[1], OUTPUT);
  pinMode(rowPins[2], OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < rows; i++) {
    int pin = rowPins[i];
    digitalWrite(pin, LOW);

    for (int j = 0; j < cols; j++) {
      int result = digitalRead(colPins[j]);
      if (result == 0) {
        Serial.print(keys[i][j]);
      }
      else
      {
        Serial.print(result);
      }
    }
    Serial.println("");

    digitalWrite(pin, HIGH);
  }

  delay(100);
}
