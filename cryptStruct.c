#include "cryptStruct.h"

// zwolnienie pamięci
void free2D(int **matrix, int size) {
  for( int i = 0; i < size; i++) free(matrix[i]);
  free(matrix);
}
// print dla lancucha
void printText(char *name, char *text, int length) {
  puts("\n*******************");
  printf_s("%s:: ", name);
  for (int i = 0; i < length; i++) printf_s("%c", text[i]);
  printf_s("\n*******************");
}
// print dla macierzy
void printMatrix(char *name, int **matrix, int size) {
  printf_s("\n***************** %s ******************\n", name);
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf_s("| %d ", matrix[i][j]);
    }
    printf_s("\n");
  }
}

//funkcji zialan
int menu () {
  char input[TEXTBUFFER];
  printf_s("\nCo chcialbys zrobic?");
  printf_s("\nenc - zaszyfrowac, dec - deszyfrowac, exit - wyjdz\n");
  gets(input);

  if (strcoll(input, "enc") == 0) return 1;
  else if (strcoll(input, "dec") == 0) return 2;
  else if (strcoll(input, "exit") == 0) return 0;
}
char * typeMessage (char *message) {
  printf_s(message);
  char *input = malloc(TEXTBUFFER * sizeof(char));
  gets(input);
  return input;
}
int typeKey () {
  char wprowadzKlucz[] = "\nWymiar macierzy dla klucza (od 3 do 7): ";
  char inp[TEXTBUFFER];
  int output;
  printf_s(wprowadzKlucz);
  gets(inp);
  output = (int)strtol(inp, NULL, 10);
  return output;
}

//dLugośc tekstu dopasowana do rozmiaru klucza
int textLength(char *text, int size) {
  int len = (int)strlen(text);
  if (len % size == 0) return len;
  len = ((int)len / (int)size) * size + size;
  return len;
}
// zwraca ustalony w zmiennych numer litery
int letToNum(char letter) {
  for (int i = 0; i < ALPHLENGTH; i++) {
    if (letter == ALPHKEYS[i][0] || letter == ALPHKEYS[i][1]) return ALPHKEYS[i][2];
  }
  return (ALPHLENGTH-1);
}
// odwrotnie, przyjmuje numer, zwraca duzą litere
char numToLet(int letKey) {
  for (int i = 0; i < ALPHLENGTH; i++) {
    if (letKey == ALPHKEYS[i][2]) {
      return ALPHKEYS[i][0];
    }
  }
  return (ALPHKEYS[ALPHLENGTH-1][0]);
}
//rozszerzenie wuch powyszych funkcji dla przetrwarzania calego lańcucha
int * convertTextToNum(char *text, int length) {
  int *textNum = malloc(length * sizeof(int));
  for (int i = 0; i < length; i++) textNum[i] = letToNum(text[i]);
  return textNum;
}
char * convertNumToText(int *numArr, int length) {
  char *text = malloc(length * sizeof(int));
  for (int i = 0; i < length; i++) text[i] = numToLet(numArr[i]);
  return text;
}
// przekształceie macierzy dwuwymiarowej w lńcuch dlawygodniejszego zapisu  plik i odwrotnie
int * convertMatrixToChain(int **key, int size) {
  int *chain = malloc(size * size * sizeof(int));
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) chain[i * size + j] = key[i][j];
  }
  return chain;
}
int ** convertChainToMatrix(int *chain, int size) {
  int **matrix = malloc(size * size * sizeof(int));
  matrix = (int**)malloc(size * sizeof(int*));
  for (int i = 0; i < size; i++) {
    matrix[i] = (int*)malloc(size * sizeof(int));
    for( int j = 0; j < size; j++) {
      matrix[i][j] = chain[i * size + j];
    }
  }
  return matrix;
}


// funkcja dla szufrowania/deszyfrowania tekstu
int * textCrypter(int **key, int keySize, int *inpText, int textLength) {
  int count = ceil((double)textLength / keySize);
  int letterNum = 0;
  int arr[keySize];
  int *text = malloc(textLength * sizeof(int));

  for(int i = 0; i < count; i++) {
    for (int j = 0; j < keySize; j++) {
      for (int x = 0; x < keySize; x++) {
        letterNum = letterNum + (inpText[i * keySize + x] * key[j][x]);
      }
      arr[j] = letterNum % ALPHLENGTH;
      letterNum = 0;
    }
    for (int j = 0; j < keySize; j++) {
      text[i * keySize + j] = arr[j];
    }
  }
  return text;
}
// losowe generuje macierz z wartościami dopasowanymi do wyiaru alfabetu
int ** matrixGenerator(int size) {
  int **key;
  int random;
  srand(time(NULL));
  key = (int**)malloc(size * sizeof(int*));
  for (int i = 0; i < size; i++) {
    key[i] = (int*)malloc(size * sizeof(int));
    for( int j = 0; j < size; j++) {

      random = rand() % ALPHLENGTH;
      key[i][j] = random;
    }
  }

  return key;
}
// liczy wyznacznik w wymiarach od 2 i wyzej poprzez metodą Laplace’a i Sarrusa
// funkcja rekursywna
int determinant(int **key, int size) {
  // liczymy wyznacznik dla macierzy
  if (size == 2) {
    int detA;
    detA = key[0][0] * key[1][1] - key[0][1] * key[1][0];
    return detA;
  }
  if (size == 3) {
    int detA = 0;

    for( int i = 0; i < size; i++ ) {
      int diagonal = 1;
      for ( int j = 0; j < size; j++) {
        int cell = (i + j) % size;
        diagonal = diagonal * key[cell][j];
      }
      detA = detA + diagonal;
    }

    for( int i = 0; i < size; i++ ) {
      int diagonal = 1;
      for ( int j = 0; j < size; j++) {
        int cell = (i + j) % size;
        diagonal = diagonal * key[cell][size - (j+1)];
      }
      detA = detA - diagonal;
    }
    return detA;
  }

  // liczymy wyznacznik metoda Cramera
  if (size > 3) {
    int detOut = 0;
    int smallerSize = size - 1;
    int ** smallerMatrix = (int**)malloc(smallerSize * sizeof(int*));
    for (int i = 0; i < smallerSize; i++) {
      smallerMatrix[i] = (int*)malloc(smallerSize * sizeof(int));
    }

    //deleted cols
    for (int i = 0; i < size; i++) {
      //dopelnienie algebraiczne
      int complement = key[0][i] * (int)pow((-1), (1 + (i + 1)));
      //rows
      for (int j = 0; j < smallerSize; j++) {
        //cols
        for (int k = 0; k < smallerSize; k++) {
          if (k < i) {
            smallerMatrix[j][k] = key[j + 1][k];
          } else if (k >= i) {
            smallerMatrix[j][k] = key[j + 1][k + 1];
          }
        }
      }
      detOut = detOut + (complement * determinant(smallerMatrix, smallerSize));
    }

    //zwalniamy pamiec
    free2D(smallerMatrix, smallerSize);
    return detOut;
  }
}
// dla uiknięcia ulamkow liczymy wyznacznik odwrotny
int inverseDeterminant(int **key, int size) {
  int det = determinant(key, size);
  if (det <= 0) return 0;

  // unikamy ułamków w macierzy odwrotnej poprzez użycie odwrotności wyznacznika na mod(ALPHLENGTH)
  // trzeba dla dezsyfrowania
  int inverseDet = 0;
  for (int i = 0; i < ALPHLENGTH; i++) {
    int detInv;
    detInv = (abs(det) * i) % ALPHLENGTH;
    if ( detInv == 1) {
      inverseDet = i;
      break;
    }
  }
  return inverseDet;
}
// ogolna funkcja lącząca w sobie poprzednie
// generuje i sprawdza macierzy dopóki nie dostaniemy macierz
// dla której możemy wyznaczyć macierz odwrotną
int ** keyGenerator(int size) {
  int inverseDet = 0;
  int **key;
  do {
    key = matrixGenerator(size);
    inverseDet = inverseDeterminant(key, size);
  }
  while(inverseDet == 0);
  return key;
}
// liczy macierz "dopełień algebraicznych" potrzebną do liczenia macierzy odwrotnej
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
// operacja "transponowana" macierzy
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
//funkcja liczy macierz odwrotną
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

// zapis zaodowanego tekstu i klucza w plik
void toSaveTextAndKey(char *text, int textLength, char *key, int keySize, char *name) {
  int let;
  FILE *file = fopen(name, "w");
  for (int i = 0; i < textLength; i++) {
    let = (int)text[i];
    fprintf_s(file, &let);
  }
  fprintf_s(file, "\n");
  for (int i = 0; i < (keySize * keySize); i++) {
    let = (int)key[i];
    fprintf_s(file, &let);
  }
  fclose(file);
}
// odczytywanie pliku z tzakodowanym tekstem i kluczem
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