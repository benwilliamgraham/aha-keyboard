const int numPins = 21;
const int pins[numPins] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 16, 17, 18, 19, 20, 21, 22, 23
};

void setup() {
  for (int pin = 0; pin < numPins; pin++)
    pinMode(pins[pin], INPUT_PULLUP);

  pinMode(13, OUTPUT);
}

void loop() {
  for (int pin = 0; pin < numPins; pin++) {
    if (digitalRead(pins[pin]) == LOW) {
      digitalWrite(13, HIGH);
      delay(50);
      digitalWrite(13, LOW);
      delay(50);
    }
  }
}
