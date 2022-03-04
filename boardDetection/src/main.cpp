#include <Arduino.h>

// Keyboard Matrix Tutorial Example
// baldengineer.com
// CC BY-SA 4.0
  
// JP1 is an input
//char *rows[] = {"A8","A9","A10","A11","A12","A13","A14","A15"};
byte rows[] = {31,33,35,37,39,41,43,45};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

byte rowsDigital[] = {2};

// JP2 and JP3 are outputs
byte cols[] = {30,32,34,36,38,40,42,44};
const int colCount = sizeof(cols)/sizeof(cols[0]);


 
byte keys[colCount][rowCount];
 
void setup() {
    Serial.begin(115200);
 
    for(int x=0; x<rowCount; x++) {
        Serial.print(rows[x]); Serial.println(" as input");
        pinMode(rows[x], INPUT);
    }
 
    for (int x=0; x<colCount; x++) {
        Serial.print(cols[x]); Serial.println(" as input-pullup");
        pinMode(cols[x], INPUT_PULLUP);
    }

    for (int i = 0; i < 7; i++){
      Serial.println(rows[i]);
    }
         
}
 
void readMatrix() {
    // iterate the columns
    for (int colIndex=0; colIndex < colCount; colIndex++) {
        // col: set to output to low
        //byte curCol = cols[colIndex];
        pinMode(cols[colIndex], OUTPUT);
        digitalWrite(cols[colIndex], LOW);
 
        // row: interate through the rows
        for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
            //byte rowCol = rows[rowIndex];
            pinMode(rows[rowIndex], INPUT_PULLUP);
            keys[colIndex][rowIndex] = digitalRead(rows[rowIndex]);
            pinMode(rows[rowIndex], INPUT);
        }
        // disable the column
        pinMode(cols[colIndex], INPUT);
    }
}
 
void printMatrix() {
    for (int rowIndex = (rowCount-1); rowIndex >= 0; rowIndex--) {
      if (rowIndex >= 0){
        Serial.print(F("0"));
      }
      Serial.print(rowIndex); Serial.print(F(": "));
      for (int colIndex = 0; colIndex < colCount; colIndex++) { 
        Serial.print(keys[colIndex][rowIndex]);
        if (colIndex < 10){
          Serial.print(F(", "));
        }
      }   
      Serial.println("");
    }
    Serial.println("    a  b  c  d  e  f  g  h");
    Serial.println("");
}
 
void loop() {
    readMatrix();
    //if (Serial.read()=='!')
    //    printMatrix();
    delay(500);
    printMatrix();
//    pinMode(cols[0], OUTPUT);
//    digitalWrite(cols[0], LOW);
//    pinMode(rowsDigital[0], OUTPUT);
//    digitalWrite(rowsDigital[0], HIGH);
//    pinMode(rows[0], INPUT);
//    keys[0][0] = analogRead(rows[0]);
//    Serial.println(analogRead(rows[0]));
}