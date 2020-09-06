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

const int _ = 0, KEY_FN = 1, KEY_HM = 2;

const int def_matrix[rows][cols] = {
    {KEY_ESC, '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=',
     KEY_BACKSPACE, _},
    {KEY_MEDIA_VOLUME_INC, KEY_TAB, 'q', 'w', 'e', 'r', 't', _, 'y', 'u', 'i',
     'o', 'p', '[', ']', '\\'},
    {KEY_MEDIA_VOLUME_DEC, KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g', _, _, 'h',
     'j', 'k', 'l', ';', '\'', KEY_RETURN},
    {KEY_PRINTSCREEN, MODIFIERKEY_SHIFT, 'z', 'x', 'c', 'v', 'b', _, _, 'b',
     'n', 'm', ',', '.', '/', MODIFIERKEY_RIGHT_SHIFT},
    {_, MODIFIERKEY_CTRL, _, MODIFIERKEY_ALT, _, KEY_SPACE, MODIFIERKEY_GUI, _,
     _, _, KEY_FN, KEY_SPACE, _, MODIFIERKEY_RIGHT_ALT, MODIFIERKEY_RIGHT_GUI,
     MODIFIERKEY_RIGHT_CTRL},
};

const int hm_matrix[rows][cols] = {
    {_, _, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', _, _, _, _},
    {KEY_MEDIA_VOLUME_INC, KEY_TAB, 'q', 'w', 'e', 'r', 't', _, 'y', 'u', 'i',
     'o', 'p', _, _, _},
    {KEY_MEDIA_VOLUME_DEC, KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g', _, _, 'h',
     'j', 'k', 'l', ';', '\'', _},
    {KEY_PRINTSCREEN, MODIFIERKEY_SHIFT, 'z', 'x', 'c', 'v', 'b', _, _, 'b',
     'n', 'm', ',', '.', '/', MODIFIERKEY_RIGHT_SHIFT},
    {_, MODIFIERKEY_CTRL, _, MODIFIERKEY_ALT, _, KEY_BACKSPACE, MODIFIERKEY_GUI,
     _, _, _, KEY_FN, KEY_SPACE, _, MODIFIERKEY_RIGHT_ALT,
     MODIFIERKEY_RIGHT_GUI, MODIFIERKEY_RIGHT_CTRL},
};

const int fn_matrix[rows][cols] = {
    {_, _, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8,
     KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_DELETE, _},
    {_, _, _, _, '=', KEY_RETURN, '~', _, _, '_', _, '[', '+', _, _, _},
    {_, _, _, _, KEY_DELETE, _, '`', _, _, KEY_LEFT, KEY_DOWN, KEY_UP,
     KEY_RIGHT, _, _, _},
    {_, MODIFIERKEY_SHIFT, _, KEY_ESC, ']', _, '\\', _, _, '\\', _, '-', _, _,
     _, _},
    {_, MODIFIERKEY_CTRL, _, MODIFIERKEY_ALT, _, KEY_BACKSPACE, MODIFIERKEY_GUI, _,
     _, _, _, KEY_DELETE, _, MODIFIERKEY_RIGHT_ALT, KEY_HM,
     MODIFIERKEY_RIGHT_CTRL},
};

char OFF = 0, DEF = 1, FN = 2, key_status[rows][cols], *fn = &key_status[4][10];

bool hm = false;

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
      int def_key = hm ? hm_matrix[row][col] : def_matrix[row][col],
          fn_key = fn_matrix[row][col];

      // update key status
      if (digitalRead(col_pins[col]) == LOW) {
        int key = (*fn) ? fn_key : def_key;
        if (!key_status[row][col]) {
          if (key > 2)
            Keyboard.press(key);
          else if (key == KEY_HM)
            hm = !hm;
          delay(5);
          key_status[row][col] = (*fn) ? FN : DEF;
        }
      } else {
        // ensure key that is released has the same fn status as when pressed
        if (key_status[row][col] == DEF && def_key > 2)
          Keyboard.release(def_key);
        else if (key_status[row][col] == FN && fn_key > 2)
          Keyboard.release(fn_key);
        key_status[row][col] = OFF;
      }
    }

    // disable row
    pinMode(row_pins[row], INPUT);
  }
}
