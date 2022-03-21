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

//enum listEtat {INITIALISATION,DEBUT,ATTENTE,LECTURE,ERREUR,ENVOI,ANALYSE,RECEPTION} etat;
enum listEtat {INITIALISATION,DEBUT,ATTENTE,LECTURE,ANALYSE,ENVOI,RECEPTION,ERREUR} etat;


const int pinButton = 8;
int etatButton = 0;
int posInit[] = {-1,-1};
int posFin[] = {-1,-1};
char move[4] = {'z','z','z','z'} ;

//p_move = &move;
int incomingByte = 0;
bool ERROR = false;

void setup() {
  Serial.begin(9600);
}

listEtat traitementMouvement(int* posInit, int* posFin, char* move, listEtat etat) {

      char liste_carc1[8] = {'a','b','c','d','e','f','g','h'};
      char liste_carc2[8] = {'1','2','3','4','5','6','7','8'};

      Serial.println("POSINIT");
      Serial.println(posInit[0]);
      Serial.println(posInit[1]);

      Serial.println("POSFINAL");
      Serial.println(posFin[0]);
      Serial.println(posFin[1]);

    if (posInit[0]>=0 && posInit[0] <8){
      move[0] = liste_carc1[posInit[0]];
     }
    else {
      etat = ERREUR;
    }

    if (posInit[1]>=0 && posInit[1] <8){
      move[1] = liste_carc2[posInit[1]];
     }
    else {
      etat = ERREUR;
    }

   if (posFin[0]>=0 && posFin[0] <8){
      move[2] = liste_carc1[posFin[0]];
     }
    else {
      etat = ERREUR;
    }

    if (posFin[1]>=0 && posInit[1] <8){
      move[3] = liste_carc2[posFin[1]];
     }
    else {
      etat = ERREUR;
    }

  if (etat != ERREUR) {
    etat = ENVOI;
  }
  Serial.print(" mouvement ");
  Serial.println(move);

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
  delay(10);
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
  Serial.print("loop ");
  Serial.println(etat);
  delay(1000);
  
  switch (etat) {
    case INITIALISATION:{

      Serial.println("INIT");
      pinMode(pinButton, INPUT);
 
      // for(int x=0; x<rowCount; x++) {
      //   Serial.print(rows[x]); Serial.println(" as input");
      //   pinMode(rows[x], INPUT);
      // }

      // for (int x=0; x<colCount; x++) {
      //   Serial.print(cols[x]); Serial.println(" as input-pullup");
      //   pinMode(cols[x], INPUT_PULLUP);
      // }

      // for (int i = 0; i < 7; i++){
      //   Serial.println(rows[i]);
      // } 

      for(int x=0; x<rowCount; x++) {
        for (int y=0; y<colCount; y++) {
          keysActu[y][x] = 0;
          keysPrec[y][x] = 0;
        }
      }

      // AJOUT BOUCLE WHILE //
      // VERIF POS PIECES DEPART //
      // ENVOI INIT DONE //

      etat = DEBUT;
    }
    break;

    case DEBUT: {

    
      Serial.println("DEBUT");
            delay(1000);

      //Mettre une couleur de led
      if (Serial.available() > 0) {
        incomingByte = Serial.read();
      }
      //etatButton = digitalRead(pinButton);
      //if (etatButton == 1) { 
      if (incomingByte == 32){
        incomingByte = 0;
        readMatrix();
        printMatrix();
        for(int x=0; x<rowCount; x++) {
          for (int y=0; y<colCount; y++) {
            keysPrec[y][x] = keysActu[y][x];
           }
        }
        etat = ATTENTE;
      }
    }
    break;

    case ATTENTE:{

    
      Serial.println("ATTENTE");
            delay(1000);

      if (Serial.available() > 0) {
        incomingByte = Serial.read();
      }
      //Mettre couleur led
      //etatButton = digitalRead(pinButton);
      //if (etatButton == 1) {
      if (incomingByte == 32){
        incomingByte = 0;  
        etat = LECTURE;
      }
    }
    break;

    case LECTURE:{

      Serial.println("LECTURE");

      //Mettre couleur led
      readMatrix();
      printMatrix();

      for(int x=0; x<rowCount; x++) {
        for (int y=0; y<colCount; y++) {
          if (keysPrec[y][x] != keysActu[y][x]) {
            if (keysActu[y][x] == 1) {
              //Erreur s'il y a plus qu'un posInit
              if (posInit[0] != -1) {
                ERROR = true;
              }
              else {
                posInit[0] = y;
                posInit[1] = x;
              }
            }
            else if (keysActu[y][x] == 0) {
              //Erreur s'il y a plus qu'un posFin
              if (posFin[0] != -1) {
                ERROR = true;
              }
              else {
                posFin[0] = y;
                posFin[1] = x;
              }
            }
          }
        }
      }

      //Erreur s'il y n'y a pas un posInit et posFin
      if (posFin[0] == -1 || posInit[0] == -1) {
        ERROR = true;
      }
      if (ERROR == false){
        etat = ANALYSE;}
      else{
        etat = ERREUR;}
    }

    break;

    case ANALYSE:
          Serial.println("ANALYSE");

      printMatrix();
      etat = traitementMouvement(posInit,posFin,move,etat);
    break;

    case ENVOI:
          Serial.println("ENVOI");

      //Envoi du mouvement au pi
      Serial.println(move);
      etat = RECEPTION;
    break;

    case RECEPTION:
    {
          Serial.println("RECEPTION");

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
        posInit[0] = -1;
        posInit[1] = -1;
        posFin[0] = -1;
        posFin[1] = -1;
        etat = ATTENTE;
      }
    }
    break;

    case ERREUR:{
    
      //Mettre couleur led
      Serial.println("ERREUR");
      posInit[0] = -1;
      posInit[1] = -1;
      posFin[0] = -1;
      posFin[1] = -1;
      etat = ATTENTE;
      ERROR = false;
    }
    break;
  }
}