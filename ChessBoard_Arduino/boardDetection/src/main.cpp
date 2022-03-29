/*
Organization: ChessBras
Autors: Emma Fréchette et Louis Vaudry
Date: March 29th, 2022

This program is used to detect the mouvement of a piece on the chessboard.
*/

#include <Arduino.h>

byte rows[] = {31,33,35,37,39,41,43,45};              //pins for each row
const int rowCount = sizeof(rows)/sizeof(rows[0]);    //number of rows

byte cols[] = {30,32,34,36,38,40,42,44};              //pins for each row
const int colCount = sizeof(cols)/sizeof(cols[0]);    //number of columns

//arrays that keeps the value of each switches (8x8 matrix)
byte keysActu[colCount][rowCount];
byte keysPrec[colCount][rowCount];

//states of the state machine
enum listEtat {INITIALISATION,DEBUT,ATTENTE,LECTURE,ANALYSE,ENVOI,RECEPTION,ERREUR} etat;

const int buttonPin = 2;              // the number of the pushbutton pin
int buttonState = 0;                  // variable for reading the pushbutton status
int posInit[] = {-1,-1};              // initial position of the piece
int posFin[] = {-1,-1};               // final position of the piece
char move[4] = {'z','z','z','z'} ;    // string of the mouvement
bool ERROR = false;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
}


listEtat traitementMouvement(int* posInit, int* posFin, char* move, listEtat etat) {
/*
  Def : This function changes the value of posInit and posFin for a real position in chess terms (ex: g7)

  Variables : posInit(int), posFin(int), move(char), etat (listEtat)

  Return : etat (listEtat)
*/

  char liste_carc1[8] = {'a','b','c','d','e','f','g','h'};
  char liste_carc2[8] = {'1','2','3','4','5','6','7','8'};

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
  return etat;
}

void readMatrix() {
/*
  Def : This function reads the value of the 64 switches and stocks it in keysActu
*/
  // iterate the columns
  for (int colIndex=0; colIndex < colCount; colIndex++) {
    // col: set to output to low
    pinMode(cols[colIndex], OUTPUT);
    digitalWrite(cols[colIndex], LOW);
 
    // row: interate through the rows
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
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
/*
  Def : This function print in the terminal the chessboard and the position of each pieces
*/
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

  delay(10);  // do NOT remove
  
  // State machine
  switch (etat) {

    case INITIALISATION:{
    /*
      This case initializes the pinMode of every output and input of the Arduino
    */

      pinMode(buttonPin, INPUT);
 
      for(int x=0; x<rowCount; x++) {
        pinMode(rows[x], INPUT);
      }

      for (int x=0; x<colCount; x++) {
        pinMode(cols[x], INPUT_PULLUP);
      }

      // sets every value of keysActu and keysPrec at 0
      for(int x=0; x<rowCount; x++) {
        for (int y=0; y<colCount; y++) {
          keysActu[y][x] = 0;
          keysPrec[y][x] = 0;
        }
      }

      etat = DEBUT;
    }
    break;

    case DEBUT: {
    /*
      This case waits for the button to be press to start the game
    */
      buttonState = digitalRead(buttonPin);

      // wait for the button the be press
      if (buttonState == HIGH){
        // scan the switches on the chessboard
        buttonState = 0;
        readMatrix();
        //printMatrix();
        // copy keysActu into keysPrec
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
    /*
      This case waits for the button to be press after a move
    */
      buttonState = digitalRead(buttonPin);
      // wait for the button the be press 
      if (buttonState == HIGH){
        buttonState = 0;
        etat = LECTURE;
      }
    }
    break;

    case LECTURE:{
    /*
      This case reads the initial position of the piece and it's final position
    */
      //Mettre couleur led
      readMatrix();
      //printMatrix();

      for(int x=0; x<rowCount; x++) {
        for (int y=0; y<colCount; y++) {
          if (keysPrec[y][x] != keysActu[y][x]) {
            if (keysActu[y][x] == 1) {
              //Error if there's more than 1 posInit
              if (posInit[0] != -1) {
                ERROR = true;
              }
              else {
                posInit[0] = y;
                posInit[1] = x;
              }
            }
            else if (keysActu[y][x] == 0) {
              //Error if there's more than 1 posFin
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

      // error if there's not a posInit or posFin
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
    /*
      This case computes the value of posInit and posFin into a string of the mouvement (ex: e3e5)
    */
      //printMatrix();
      etat = traitementMouvement(posInit,posFin,move,etat);
    break;

    case ENVOI:
    /*
      This case sends the string of the mouvement to the Raspberry Pi
    */
      //Envoi du mouvement au pi
      Serial.println(move);
      Serial.flush();
      etat = RECEPTION;
    break;

    case RECEPTION:
    /*
      This case waits for a response of the Rapsberry Pi if the mouvement was legal
    */
      bool OK = true;  

      //Lecture de la valeur de retour
      // if (Serial.available() > 0) {
      //   OK = Serial.read();
      // }

      if (OK == false) {
        etat = ERREUR;
        break;
      }
      else {
        // copy keysActu into keysPrec
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
    break;

    case ERREUR:
    /*
      This case resets the value of posInit, posFin and ERROR
    */
      posInit[0] = -1;
      posInit[1] = -1;
      posFin[0] = -1;
      posFin[1] = -1;
      etat = ATTENTE;  // return to ATTENTE for the player to do a correct move
      ERROR = false;
    break;
  }
}