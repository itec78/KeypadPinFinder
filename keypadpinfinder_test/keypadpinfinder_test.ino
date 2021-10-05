/* @file HelloKeypad.pde
  || @version 1.0
  || @author Alexander Brevig
  || @contact alexanderbrevig@gmail.com
  ||
  || @description
  || | Demonstrates the simplest use of the matrix Keypad library.
  || #
*/
#include <Keypad.h>

// ---------- Paste here ----------


const byte rows = 4; // rows
const byte cols = 3; // columns
char keys[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[rows] = {17, 10, 6, 19}; //connect to the row pinouts of the keypad
byte colPins[cols] = {9, 12, 16}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);


// ---------- End paste ----------



void setup() {
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Keypad pin finder (test)");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);
  }
}
