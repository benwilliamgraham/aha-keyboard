/* Keyboard layout:
 *   Sends signals down vertical (col) wires (0-12, 17, 18)
 *   Recieves signals from horizontal (row) wires (19-23)
 */

#include <Keyboard.h>

const int rows = 5, cols = 16;

const int row_pins[rows] = {0, 1, 2, 3, 4},
          col_pins[cols] = {
              5,  6,  7,  8,  9,  10, 12, 11, // left half
              23, 22, 21, 20, 19, 18, 17, 16  // right half
};

const int KEY_NONE = 0, KEY_FN = 1;

const int def_matrix[rows][cols] = {
    {KEY_ESC, '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
     KEY_BACKSPACE, KEY_NONE},
    {KEY_MEDIA_VOLUME_INC, KEY_TAB, 'q', 'w', 'e', 'r', 't', KEY_NONE, 'y', 'u',
     'i', 'o', 'p', '[', ']', '\\'},
    {KEY_MEDIA_VOLUME_DEC, KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g', KEY_NONE,
     KEY_NONE, 'h', 'j', 'k', 'l', ';', '\'', KEY_RETURN},
    {KEY_PRINTSCREEN, MODIFIERKEY_SHIFT, 'z', 'x', 'c', 'v', 'b', KEY_NONE,
     KEY_NONE, 'b', 'n', 'm', ',', '.', '/', MODIFIERKEY_RIGHT_SHIFT},
    {KEY_NONE, MODIFIERKEY_CTRL, KEY_NONE, MODIFIERKEY_ALT, KEY_NONE, KEY_SPACE,
     MODIFIERKEY_GUI, KEY_NONE, KEY_NONE, KEY_NONE, KEY_FN, KEY_SPACE, KEY_NONE,
     MODIFIERKEY_RIGHT_ALT, MODIFIERKEY_RIGHT_GUI, MODIFIERKEY_RIGHT_CTRL},
};

const int fn_matrix[rows][cols] = {
    {KEY_NONE, KEY_NONE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7,
     KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_DELETE, KEY_NONE},
    {KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, '=', KEY_NONE, '~',
     KEY_NONE, KEY_NONE, '_', KEY_NONE, '[', '+', KEY_NONE,
     KEY_NONE, KEY_NONE},
    {KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_DELETE, KEY_NONE, '`',
     KEY_NONE, KEY_NONE, KEY_LEFT, KEY_DOWN, KEY_UP, KEY_RIGHT, KEY_NONE,
     KEY_NONE, KEY_NONE},
    {KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, ']', KEY_NONE, '\\',
     KEY_NONE, KEY_NONE, '\\', KEY_NONE, '-', KEY_NONE, KEY_NONE,
     KEY_NONE, KEY_NONE},
    {KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_BACKSPACE, KEY_NONE,
     KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE, KEY_NONE,
     KEY_NONE, KEY_NONE},
};

char OFF = 0, DEF = 1, FN = 2, key_status[rows][cols], *fn = &key_status[4][10];

void setup() {
  Keyboard.begin();

  // initialize coms
  for (int row = 0; row < rows; row++)
    pinMode(row_pins[row], INPUT);

  for (int col = 0; col < cols; col++)
    pinMode(col_pins[col], INPUT_PULLUP);

  // create key status listing
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++)
      key_status[row][col] = OFF;
  }
}

void loop() {
  // loop through keys
  for (int row = 0; row < rows; row++) {
    // enable row
    pinMode(row_pins[row], OUTPUT);
    digitalWrite(row_pins[row], LOW);

    // iterate through rows
    for (int col = 0; col < cols; col++) {
      int def_key = def_matrix[row][col], fn_key = fn_matrix[row][col];

      // update key status
      if (digitalRead(col_pins[col]) == LOW) {
        int key = (*fn) ? fn_key : def_key;
        if (!key_status[row][col] && key > 1)
          Keyboard.press(key);
        delay(5);
        key_status[row][col] = (*fn) ? FN : DEF;
      } else {
        // ensure key that is released has the same fn status as when pressed
        if (key_status[row][col] == DEF && def_key > 1)
          Keyboard.release(def_key);
        else if (key_status[row][col] == FN && fn_key > 1)
          Keyboard.release(fn_key);
        key_status[row][col] = OFF;
      }
    }

    // disable row
    pinMode(row_pins[row], INPUT);
  }
}
