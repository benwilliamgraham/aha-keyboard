int in_pin = 9;
int key = MODIFIERKEY_CTRL;

void setup() {
  Keyboard.begin();
  pinMode(in_pin, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {
  if (digitalRead(in_pin) == LOW) {
      Keyboard.press(key);
      digitalWrite(13, HIGH);
      delay(100);
      Keyboard.release(key);
      digitalWrite(13, LOW);
      delay(100);
  }
}
