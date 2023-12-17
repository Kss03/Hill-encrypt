#include <stdio.h>
#include <stdlib.h>
#include "variables.h"
#include <time.h>
#include <math.h>



int letToNum(char letter) {
    for (int i = 0; i < alphLength; i++) {
        if (letter == letKeys[i][0] || letter == letKeys[i][1]) {
            return letKeys[i][2];
        }
    }
    printf_s("\nPlease, provide correct data\n");
    return -1;
}

char numToLet(int num) {
    for (int i = 0; i < alphLength; i++) {
        if (num == letKeys[i][2]) {
            return letKeys[i][0];
        }
    }
    printf_s("\nPlease, provide correct data\n");
    return -1;
}

int ** keyEncGenerator(int size) {
    // с помощью malloc выделяем кусок памяти => кол-во элементов * размер типа int
    // za pomocą malloc przydzielamy pamięć dla lańcuchu => ilość elementów * wymiar typu "int"
    int ** keyEnc = (int**)malloc(size * sizeof(int*));

    //dodatkowe dla funkcji rand() - zeby nie zwracała to samo
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        // и для каждого вложенного массива также специально выделяем память
        // i dla każdego zagniezdzionego lańcucha także przydzielamy pamięć
        keyEnc[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            // wypełniamy lańcuch znaczeniami

            int randFrom0to25 = rand() % 26;
            keyEnc[i][j] = randFrom0to25;
        }
    }
    // все эти манипуляции с памятью только для того, чтобы вернуть и использовать далее полученную матрицу из функции
    // to wszystko dla tego, żeby zwrocić z funkcji tą macierz
    // ps. nie zapominamy o zwolnieniu pamięci za pomocą free(arrName)
    // ps. не забываем потом очистить память вручную с помощью free(arrName); как по пользуемся
    return keyEnc;
}

int determinant ( int **matrix, int size) {
    // liczymy wyznacznik dla macierzy
    if (size == 2) {
        int detA;

        detA = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        return detA;
    }
    if (size == 3) {
        int detA = 0;

        for( int i = 0; i < size; i++ ) {
            int diagonal = 1;
            for ( int j = 0; j < size; j++) {
                int cell = (i + j) % size;
                diagonal = diagonal * matrix[cell][j];
            }
            detA = detA + diagonal;
        }

        for( int i = 0; i < size; i++ ) {
            int diagonal = 1;
            for ( int j = 0; j < size; j++) {
                int cell = (i + j) % size;
                diagonal = diagonal * matrix[cell][size - (j+1)];
            }
            detA = detA - diagonal;
        }

        return detA;
    }
    if (size > 3) {
        int detOut = 0;
        int detA;

        int smallerSize = size - 1;
        int ** smallerMatrix = (int**)malloc(smallerSize * sizeof(int*));
        for (int i = 0; i < smallerSize; i++) {
            smallerMatrix[i] = (int*)malloc(smallerSize * sizeof(int));
        }

        //deleted cols
        for (int i = 0; i < size; i++) {
            //dopelnienie algebraiczne
            int complement = matrix[0][i] * (int)pow((-1), (1 + (i + 1)));

            //rows
            for (int j = 0; j < smallerSize; j++) {
                //cols
                for (int k = 0; k < smallerSize; k++) {
                    if (k < i) {
                        smallerMatrix[j][k] = matrix[j + 1][k];
                    } else if (k >= i) {
                        smallerMatrix[j][k] = matrix[j + 1][k + 1];
                    }

                }

            }
            detOut = detOut + (complement * determinant(smallerMatrix, smallerSize));
        }

        //zwalniamy pamiec
        for( int i = 0; i < smallerSize; i++) {
            free(smallerMatrix[i]);
        }
        free(smallerMatrix);
        return detOut;
    }
}
