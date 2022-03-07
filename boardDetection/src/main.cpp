// Keyboard Matrix Tutorial Example
// baldengineer.com
// CC BY-SA 4.0

#include <Arduino.h>
//#include "traitementMouvement.h"
  
// JP1 is an input
//char *rows[] = {"A8","A9","A10","A11","A12","A13","A14","A15"};
byte rows[] = {31,33,35,37,39,41,43,45};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

byte rowsDigital[] = {2};

// JP2 and JP3 are outputs
byte cols[] = {30,32,34,36,38,40,42,44};
const int colCount = sizeof(cols)/sizeof(cols[0]);

byte keysActu[colCount][rowCount];
byte keysPrec[colCount][rowCount];

enum listEtat {INITIALISATION,DEBUT,ATTENTE,LECTURE,ERREUR,ENVOI,ANALYSE,RECEPTION} etat;


const int pinButton = 8;
int etatButton = 0;
int posInit[] = {-1,-1};
int posFin[] = {-1,-1};
//char mouvement[] = {'z','z','z','z'};
char move[] = "zzzz";
//p_move = &move;

void setup() {
  Serial.begin(115200);
}

listEtat traitementMouvement(int* posInit, int* posFin, char* move, listEtat etat) {
    //enum listEtat {INITIALISATION,DEBUT,ATTENTE,LECTURE,ERREUR,ENVOI,ANALYSE,RECEPTION};
    //char move[] = {'z','z','z','z'};
    if (posInit[0] == 0) {
      move[0] = 'a';
    }
    else if (posInit[0] == 1) {
      move[0] = 'b';
    }
    else if (posInit[0] == 2) {
      move[0] = 'c';
    }
    else if (posInit[0] == 3) {
      move[0] = 'd';
    }
    else if (posInit[0] == 4) {
      move[0] = 'e';
    }
    else if (posInit[0] == 5) {
      move[0] = 'f';
    }
    else if (posInit[0] == 6) {
      move[0] = 'g';
    }
    else if (posInit[0] == 7) {
      move[0] = 'h';
    }
    else {
      etat = ERREUR;
    }
    if (posInit[1] == 0) {
      move[1] = '1';
    }
    else if (posInit[1] == 1) {
      move[1] = '2';
    }
    else if (posInit[1] == 2) {
      move[1] = '3';
    }
    else if (posInit[1] == 3) {
      move[1] = '4';
    }
    else if (posInit[1] == 4) {
      move[1] = '5';
    }
    else if (posInit[1] == 5) {
      move[1] = '6';
    }
    else if (posInit[1] == 6) {
      move[1] = '7';
    }
    else if (posInit[1] == 7) {
      move[1] = '8';
    }
    else {
      etat = ERREUR;
    }
    if (posFin[0] == 0) {
      move[2] = 'a';
    }
    else if (posFin[0] == 1) {
      move[2] = 'b';
    }
    else if (posFin[0] == 2) {
      move[2] = 'c';
    }
    else if (posFin[0] == 3) {
      move[2] = 'd';
    }
    else if (posFin[0] == 4) {
      move[2] = 'e';
    }
    else if (posFin[0] == 5) {
      move[2] = 'f';
    }
    else if (posFin[0] == 6) {
      move[2] = 'g';
    }
    else if (posFin[0] == 7) {
      move[2] = 'h';
    }
    else {
      etat = ERREUR;
    }
    if (posFin[1] == 0) {
      move[4] = '1';
    }
    else if (posFin[1] == 1) {
      move[4] = '2';
    }
    else if (posFin[1] == 2) {
      move[4] = '3';
    }
    else if (posFin[1] == 3) {
      move[4] = '4';
    }
    else if (posFin[1] == 4) {
      move[4] = '5';
    }
    else if (posFin[1] == 5) {
      move[4] = '6';
    }
    else if (posFin[1] == 6) {
      move[4] = '7';
    }
    else if (posFin[1] == 7) {
      move[4] = '8';
    }
    else {
      etat = ERREUR;
    }
  if (etat != ERREUR) {
    etat = ENVOI;
  }
  return etat;
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
      keysActu[colIndex][rowIndex] = digitalRead(rows[rowIndex]);
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
      Serial.print(keysActu[colIndex][rowIndex]);
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
  switch (etat) {
    case INITIALISATION:
      pinMode(pinButton, INPUT);
 
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

      for(int x=0; x<rowCount; x++) {
        for (int y=0; y<colCount; y++) {
          keysActu[y][x] = 0;
          keysPrec[y][x] = 0;
        }
      }

      etat = DEBUT;

    break;

    case DEBUT:
      //Mettre une couleur de led
      etatButton = digitalRead(pinButton);
      if (etatButton == 1) {
        readMatrix();
        for(int x=0; x<rowCount; x++) {
          for (int y=0; y<colCount; y++) {
            keysPrec[y][x] = keysActu[y][x];
          }
        }
        etat = ATTENTE;
      }
    break;

    case ATTENTE:
      //Mettre couleur led
      etatButton = digitalRead(pinButton);
      if (etatButton == 1) {
        etat = LECTURE;
      }
    break;

    case LECTURE:
      //Mettre couleur led
      readMatrix();
      for(int x=0; x<rowCount; x++) {
        for (int y=0; y<colCount; y++) {
          if (keysPrec[y][x] != keysActu[y][x]) {
            if (keysActu[y][x] == 1) {
              //Erreur s'il y a plus qu'un posInit
              if (posInit[0] != -1) {
                etat = ERREUR;
                break;
              }
              else {
                posInit[0] = colCount;
                posInit[1] = rowCount;
              }
            }
            else if (keysActu[y][x] == 0) {
              //Erreur s'il y a plus qu'un posFin
              if (posFin[0] != -1) {
                etat = ERREUR;
                break;
              }
              else {
                posFin[0] = colCount;
                posFin[1] = rowCount;
              }
            }
          }
        }
      }
      //Erreur s'il y n'y a pas un posInit et posFin
      if (posFin[0] == -1 || posInit[0] == -1) {
        etat = ERREUR;
        break;
      }
      else {
        etat = ANALYSE;
      }
    break;

    case ANALYSE:
      etat = traitementMouvement(posInit,posFin,move,etat);
    break;

    case ENVOI:
      //Envoi du mouvement au pi
      Serial.println(move);
      etat = RECEPTION;
    break;

    case RECEPTION:
      //Lecture de la valeur de retour
      bool OK = true;
      if (OK == false) {
        etat = ERREUR;
        break;
      }
      else {
        //Copie de Actu dans Prec
        for(int x=0; x<rowCount; x++) {
          for (int y=0; y<colCount; y++) {
            keysPrec[y][x] = keysActu[y][x];
          }
        }
        etat = ATTENTE;
      }
    break;

    case ERREUR:
      //Mettre couleur led
      int posInit[] = {-1,-1};
      int posFin[] = {-1,-1};
      etat = ATTENTE;
    break;
  }
}