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
const byte cols = 4; // columns
char keys[rows][cols] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[rows] = {3, 4, 5, 6}; //connect to the row pinouts of the keypad
byte colPins[cols] = {8, 9, 10, 11}; //connect to the column pinouts of the keypad
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
