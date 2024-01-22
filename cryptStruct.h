#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <string.h>

// nie wiem co to, utworzylo IDE :)
//#ifndef NEW_VARIABLES_H
//#define NEW_VARIABLES_H
//
//#endif //NEW_VARIABLES_H


#define TEXTBUFFER 250
#define ALPHLENGTH 29

//kluczy dla alphabetu
const char ALPHKEYS[ALPHLENGTH][3] = {
    {'A', 'a', 0},
    {'B', 'b', 1},
    {'C', 'c', 2},
    {'D', 'd', 3},
    {'E', 'e', 4},
    {'F', 'f', 5},
    {'G', 'g', 6},
    {'H', 'h', 7},
    {'I', 'i', 8},
    {'J', 'j', 9},
    {'K', 'k', 10},
    {'L', 'l', 11},
    {'M', 'm', 12},
    {'N', 'n', 13},
    {'O', 'o', 14},
    {'P', 'p', 15},
    {'Q', 'q', 16},
    {'R', 'r', 17},
    {'S', 's', 18},
    {'T', 't', 19},
    {'U', 'u', 20},
    {'V', 'v', 21},
    {'W', 'w', 22},
    {'X', 'x', 23},
    {'Y', 'y', 24},
    {'Z', 'z', 25},
    {'.', '.', 26},
    {',', ',', 27},
    {' ', ' ', 28}
};

//struktura dla tekstu
struct Message {
  int length;
  char *inputText;
  char *outText;
  int *textDigRep;
};

// struktura dla klucza
struct Keys {
  int size;
  char *keyTextRep;
  int *keyChain;
  int **encryptKey;
  int **decryptKey;
};

//ogolna struktura dla szyfrowania\deszyfrowania
struct Session {
  struct Message message;
  struct Keys keys;
  char *fileName;
};

//dodatkowe funkcji
void free2D(int **matrix, int size);
//
void printText(char *name, char *text, int length);
void printMatrix(char *name, int **matrix, int size);

int menu ();
char * typeMessage ();
int typeKey ();

// funkcji dla przeksztalcenia danych
int textLength(char *text, int size); //dLugośc tekstu dopasowana do wymiaru klucza
int letToNum(char letter);
char numToLet(int letKey);
int * convertTextToNum(char *text, int length);
char * convertNumToText(int *numArr, int length);
int * convertMatrixToChain(int **key, int size);
int ** convertChainToMatrix(int *chain, int size);

//matematyczne operacji
// funkcja dla szufrowania/deszyfrowania tekstu
int * textCrypter(int **key, int keySize, int *inpText, int textLength);//szyfruje tekst za pomoca matrycy
int ** matrixGenerator(int size);
int determinant(int **key, int size);
int inverseDeterminant(int **key, int size);
int ** keyGenerator(int size);
//funkcja liczy macierz odwrotną
int **algebraicComplementMatrix (int **matrix, int size);
int ** transposition (int **matrix, int size);
int **findDecryptKey(int **key, int size);

// zapis wynikow w plik
void toSaveTextAndKey(char *text, int textLength, char *key, int keySize, char *name);
int toReadFile (char *fileName, char *text, char *key);