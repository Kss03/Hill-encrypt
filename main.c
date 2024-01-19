#include "cryptStruct.c"


void encryptMessage();

int toReadFile (char *fileName, char *text, char *key) {
  char line[TEXTBUFFER];
  FILE *file = fopen(fileName, "r");
  if (!file) return 0;
  fgets(text, TEXTBUFFER, file);
  text[strlen(text) - 1] = 0;
  fgets(key, TEXTBUFFER, file);
  fclose(file);
  return 1;
}

//funkcja liczy macierz odwrotną
int **algebraicComplementMatrix (int **matrix, int size) {
  int ** complementMatrix = (int**)malloc(size * sizeof(int*));
  for (int i = 0; i < size; i++) {
    complementMatrix[i] = (int*)malloc(size * sizeof(int));
  }

  int smallerSize = size - 1;
  //pamiec do macierzy dopelmien algebraicznych
  int ** matrixB = (int**)malloc((smallerSize) * sizeof(int*));
  for (int i = 0; i < smallerSize; i++) {
    matrixB[i] = (int*)malloc(smallerSize * sizeof(int));
  }

  for (int i = 0; i < size; i++) {

    for(int j = 0; j < size; j++) {
      //dopelnienie algebraiczne
      int negativePositive = (int)pow((-1), ((i + 1) + (j + 1)));

      for( int k = 0; k < smallerSize; k++) {
        for( int l = 0; l < smallerSize; l++) {

          if (k < i) {
            if (l < j) matrixB[k][l] = matrix[k][l];
            else if (l >= j) matrixB[k][l] = matrix[k][l + 1];
          } else if (k >= i) {
            if (l < j) matrixB[k][l] = matrix[k + 1][l];
            else if (l >= j) matrixB[k][l] = matrix[k + 1][l + 1];
          }

        }
      }

      //liczymy wyznacznik dla wszystkich dopełnień macierzy odwrotnej
      complementMatrix[i][j] = negativePositive * determinant(matrixB, smallerSize);
      // modulo dla każdego znaczenia macierzy ( unikamy liczb ujemnych poprzez dodawanie do niej znaczenie modulo
      if (complementMatrix[i][j] < 0) complementMatrix[i][j] = (complementMatrix[i][j] % ALPHLENGTH) + ALPHLENGTH;
      else complementMatrix[i][j] = complementMatrix[i][j] % ALPHLENGTH;
    }
  }
  free2D(matrixB, smallerSize);
  return complementMatrix;
}

int ** transposition (int **matrix, int size) {
  int newMatrix[size][size];
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      // robimy transponowanie
      newMatrix[i][j] = matrix[j][i];
    }
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      // robimy transponowanie
      matrix[i][j] = newMatrix[i][j];
    }
  }
  return matrix;
}

int **findDecryptKey(int **key, int size) {
  int ** decKey;
  int inverseDet = inverseDeterminant(key, size);
  decKey = algebraicComplementMatrix(key, size);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      decKey[i][j] = (inverseDet * decKey[i][j]) % ALPHLENGTH;
    }
  }
  transposition(decKey, size);
  return decKey;
}

void decryptMessage() {
  struct Session session;
  struct Message msg = session.message;
  struct Keys keys = session.keys;
  // 0 - plik nie istnieje, 1 - OK, 2 - EXIT
  int isFileExist;

  msg.inputText = malloc(TEXTBUFFER * sizeof(char));
  keys.keyTextRep = malloc(TEXTBUFFER * sizeof(char));
  session.fileName = typeMessage("\nWprowadz nazwe pliku (np. text.txt) albo exit aby przerwac program: ");
  if (strcmp(session.fileName, "exit") == 0) return;
  printf_s(session.fileName);
  isFileExist = toReadFile(session.fileName, msg.inputText, keys.keyTextRep);

  if (isFileExist == 0) {
    do {
      printf_s("\n!!! plik o nazwie '%s' nie istnieje...", session.fileName);

      session.fileName = typeMessage("\nWprowadz nazwe pliku albo 'exit' aby przerwac program: ");
      if (strcmp(session.fileName, "exit") == 0) {
        isFileExist = 2;
        break;
      }
      printf_s(session.fileName);
      isFileExist = toReadFile(session.fileName, msg.inputText, keys.keyTextRep);
    } while (isFileExist == 0);
  }
  if (isFileExist == 2) return;

  //przetrwarzamy klucz
  keys.size = (int)sqrt((double)strlen(keys.keyTextRep));
  keys.keyChain = convertTextToNum(keys.keyTextRep, keys.size * keys.size);
  keys.encryptKey = convertChainToMatrix(keys.keyChain, keys.size);
  keys.decryptKey = findDecryptKey(keys.encryptKey, keys.size);
  printMatrix("Klucz dla deszyfrowania", keys.decryptKey, keys.size);

  //przetrwarzamy tekst
  msg.length = textLength(msg.inputText, keys.size);
  msg.textDigRep = convertTextToNum(msg.inputText, msg.length);
  msg.textDigRep = textCrypter(
    keys.decryptKey,
    keys.size,
    msg.textDigRep,
    msg.length
  );
  msg.outText = convertNumToText(msg.textDigRep, msg.length);
  printf_s("%d", msg.length);
  printText("encrypted text", msg.outText, msg.length);


  free2D(keys.decryptKey, keys.size);
  free2D(keys.encryptKey, keys.size);
  free(keys.keyChain);
  free(keys.keyTextRep);
  free(msg.inputText);
  free(msg.outText);
  free(msg.textDigRep);
  free(session.fileName);
}

int main() {

  while(1) {
    int operation = menu();

    if (operation == 1) encryptMessage();
    if (operation == 2) decryptMessage();
    if (operation == 0) return 0;
  }

  system("pause");
  return 0;
}

void encryptMessage() {
  struct Session session;
  struct Message msg = session.message;
  struct Keys keys = session.keys;

  char message[] = "\nWprowadz text (litery Aa-Zz): ";
  char message2[] = "\nWprowadz nazwe pliku (np. text.txt): ";
  msg.inputText = typeMessage(message);
  keys.size = typeKey();
  session.fileName = typeMessage(message2);
  msg.length = textLength(msg.inputText, keys.size);
  msg.textDigRep = convertTextToNum(msg.inputText, msg.length);
  //generujemy klucz
  keys.encryptKey = keyGenerator(keys.size);
  printMatrix("encrypt key", keys.encryptKey, keys.size);

  //szyfrujemy tekst
  msg.textDigRep = textCrypter(
      keys.encryptKey,
      keys.size,
      msg.textDigRep,
      msg.length
  );

  // dalej zapiszemy klucz i tekst w "tekst" dla dalszego zapisywania w plik
  //key
  keys.keyChain = convertMatrixToChain(keys.encryptKey, keys.size);
  keys.keyTextRep = convertNumToText(keys.keyChain, keys.size * keys.size);
  // text
  msg.outText = convertNumToText(msg.textDigRep, msg.length);
  printText("klucz w formacie tekstu", keys.keyTextRep, (keys.size * keys.size));
  printText("zaszyfrowany tekst", msg.outText, msg.length);
  //zapisujemy wyniki w plik
  toSaveTextAndKey(
      msg.outText,
      msg.length,
      keys.keyTextRep,
      keys.size,
      session.fileName
  );

  free(msg.textDigRep);
  free(msg.inputText);
  free(msg.outText);
  free(keys.keyTextRep);
  free(keys.keyChain);
  free(session.fileName);
  free2D(keys.encryptKey, keys.size);
}