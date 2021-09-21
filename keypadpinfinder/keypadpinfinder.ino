
// Keypad pin finder
// by itec (itec@ventuordici.org)
//
// Instructions:
// Connect your keypad to pins
// Open serial console
// Input keys list, or leave empty for default
// Press keypad keys
// Copy and paste the code!



int myPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, A1, A2, A3, A4, A5};
String keys = "1234567890*#ABCD";
bool debug = false;

char arr[sizeof(myPins) / sizeof(myPins[0])][sizeof(myPins) / sizeof(myPins[0])];
int numpins = sizeof(myPins) / sizeof(myPins[0]);

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("*****************");
  Serial.println("Keypad pin finder");
  Serial.println("*****************");
  Serial.println();

  //Serial.println("Press key '" + String(key[0]) + "'");

  //Set input pins
  for (byte i = 0; i < sizeof(myPins) / sizeof(myPins[0]); i++) {
    pinMode(myPins[i], INPUT_PULLUP);
    //Serial.println(String(i) + " " + myPins[i] + " " + digitalRead(myPins[i]));
  }
}

String waitserialinput() {
  String st = "";
  byte ch;
  while (true) {
    if (Serial.available()) {
      ch = Serial.read();
      st += (char)ch;
      if (ch == '\n') { // Command recevied and ready.
        st.trim();
        return (st);
      }
    }
  }
}

void printarray() {
  //Stampa l'array
  for (byte a = 0; a < numpins; a++) {
    for (byte b = 0; b < numpins; b++) {
      if (arr[a][b] == NULL) {
        Serial.print("-");
      } else {
        Serial.print(arr[a][b]);
      }
    }
    Serial.println();

  }
  Serial.println();
}





void loop() {

  //Prompts for user defined key list
  Serial.println("Enter new keys list or empty to continue [" + keys + "]");

  String usrkeys = waitserialinput();
  if (usrkeys != "") {
    keys = usrkeys;
  }
  Serial.println(keys);
  char key[keys.length() + 1];
  keys.toCharArray(key, keys.length() + 1);



  String lastcouple = "";
  int ncouple = 0;
  //reset array
  for (byte a = 0; a < numpins; a++) {
    for (byte b = 0; b < numpins; b++) {
      arr[a][b] = NULL;
    }
  }

  //Loop setting one pin to output
  while (ncouple < keys.length()) {
    bool pressed = false;
    Serial.println("Press key '" + String(key[ncouple]) + "' on keypad");

    while (not pressed) {
      for (byte a = 0; a < numpins - 1; a++) {
        pinMode(myPins[a], OUTPUT);
        digitalWrite(myPins[a], LOW);

        for (byte b = a + 1; b < numpins; b++) {
          if (digitalRead(myPins[b]) == LOW) {

            //Debounce
            String couple = String(a) + " " + String(b);
            if (couple != lastcouple) {
              Serial.println("Pins: " + String(myPins[a]) + " " + String(myPins[b]));
              lastcouple = couple;

              arr[a][b] = key[ncouple];
              ncouple++;
              pressed = true;
              //Serial.println("Press key '" + String(key[ncouple]) + "'");
            }
          }
        }
        pinMode(myPins[a], INPUT_PULLUP);
      }
    }
  }


  Serial.println();

  //ok, time to process data

  if (debug) {
    printarray();
  }

  //Fill diagonal
  for (byte a = 0; a < numpins; a++) {
    for (byte b = a; b < numpins; b++) {
      arr[b][a] = arr[a][b];
    }
  }

  if (debug) {
    printarray();
  }

  //delete duplicates
  for (byte a = 0; a < numpins; a++) {
    for (byte b = a; b < numpins; b++) {
      if (arr[a][b] != NULL) {
        for (byte i = 0; i < numpins; i++) {
          arr[b][i] = NULL;
          arr[i][a] = NULL;
        }
      }
    }
  }

  if (debug) {
    printarray();
  }

  //Delete empty rows
  int PinRow[sizeof(myPins) / sizeof(myPins[0])];
  int numrow = 0;

  for (byte a = 0; a < sizeof(myPins) / sizeof(myPins[0]); a++) {
    bool isempty = true;
    for (byte b = 0; b < sizeof(myPins) / sizeof(myPins[0]); b++) {
      arr[numrow][b] = arr[a][b];

      if (arr[a][b] != NULL) {
        isempty = false;
        if (numrow != a) {
          arr[a][b] = NULL;
        }
      }
    }
    if (not isempty) {
      PinRow[numrow] = myPins[a];
      numrow++;
    }
  }

  //Delete empty columns
  int PinCol[sizeof(myPins) / sizeof(myPins[0])];
  int numcol = 0;

  for (byte b = 0; b < sizeof(myPins) / sizeof(myPins[0]); b++) {
    bool isempty = true;
    for (byte a = 0; a < sizeof(myPins) / sizeof(myPins[0]); a++) {
      arr[a][numcol] = arr[a][b] ;
      if (arr[a][b] != NULL) {
        isempty = false;
        if (numcol != b) {
          arr[a][b] = NULL;
        }
      }
    }
    if (not isempty) {
      PinCol[numcol] = myPins[b];
      numcol++;
    }
  }

  if (debug) {
    printarray();
  }


  //Print code!!!
  String s, t;
  Serial.println("Copy and paste following lines into your code");
  Serial.println("---------------------------------------------");
  Serial.println();

  Serial.println("const byte rows = " + String(numrow) + "; // rows");
  Serial.println("const byte cols = " + String(numcol) + "; // columns");

  Serial.println("char keys[rows][cols] = {");
  t = "";
  for (byte i = 0; i < numrow; i++) {
    s = "";
    for (byte j = 0; j < numcol; j++) {
      s = s + ",'" + String(arr[i][j]) + "'";
    }
    s.remove(0, 1);
    t = t + ",\n{" + s + "}";
  }
  t.remove(0, 2);
  t.replace("''", "NULL");
  Serial.println(t);
  Serial.println("};");

  s = "";
  for (byte i = 0; i < numrow; i++) {
    s = s + ", " + String(PinRow[i]);
  }
  s.remove(0, 2);
  Serial.println("byte rowPins[rows] = {" + s + "}; //connect to the row pinouts of the keypad");


  s = "";
  for (byte i = 0; i < numcol; i++) {
    s = s + ", " + String(PinCol[i]);
  }
  s.remove(0, 2);
  Serial.println("byte colPins[cols] = {" + s + "}; //connect to the column pinouts of the keypad");

  Serial.println("Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);");

  Serial.println();
  Serial.println("---------------------------------------------");
  Serial.println();


  //end

}





/*
  const byte rows = 4; //four rows
  const byte cols = 3; //three columns
  char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'
  #','0','*'}
  };
  byte rowPins[rows] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
  byte colPins[cols] = {8, 7, 6}; //connect to the column pinouts of the keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
*/