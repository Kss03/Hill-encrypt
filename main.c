#include <cscapi.h> //pause function
#include "functions.h"
#include <stdbool.h>


int inverseDeterminant (int **matrix, int size) {
    //jezeli wyznacznik == 0 zmieniamy macierz
    int det = determinant(matrix, size);
    if (det == 0) return 0;

    // unikamy ułamków w macierzy odwrotnej poprzez użycie odwrotności wyznacznika na mod26
    // trzeba dla dezsyfrowania
    int inverseDet;
    for (int i = 0; i < alphLength; i++) {

        int detInv = (abs(det) * i) % 26;
        if ( detInv == 1) {
            inverseDet = i;
        }
    }
    return inverseDet;
}

int **algebraicComplementMatrix (int **matrix, int size) {

    int ** complementMatrix = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        complementMatrix[i] = (int*)malloc(size * sizeof(int));
    }

    int smallerSize = size - 1;
    int ** matrixB = (int**)malloc((smallerSize) * sizeof(int*));
    for (int i = 0; i < smallerSize; i++) {
        matrixB[i] = (int*)malloc(smallerSize * sizeof(int));
    }
    printf_s("\n######## dopełnienia algebraiczne #########\n");
    for (int i = 0; i < size; i++) {

        for(int j = 0; j < size; j++) {
            //dopelnienie algebraiczne
            int negativePositive = (int)pow((-1), ((i + 1) + (j + 1)));

            for( int k = 0; k < smallerSize; k++) {
                for( int l = 0; l < smallerSize; l++) {
                    if (k < i) {
                        if (l < j) matrixB[k][l] = matrix[k][l];
                        else if (l >= j) matrixB[k][l] = matrix[k][l+1];
                    } else if (k >= i) {
                        if (l < j) matrixB[k][l] = matrix[k+1][l];
                        else if (l >= j) matrixB[k][l] = matrix[k+1][l+1];
                    }
                }
            }

            //liczymy wyznacznik dla wszystkich dopełnień macierzy odwrotnej
            // Od razu transponujemy (ij -> ji)
            complementMatrix[j][i] = negativePositive * determinant(matrixB, smallerSize);


            printf_s("+%d+", complementMatrix[i][j]);

        }
        printf_s("\n");
    }
    free(matrixB);
    return complementMatrix;
}

int **inverseMatrix (int **matrix, int size, int inverseDet) {
    int **invMatrix;
    invMatrix = algebraicComplementMatrix(matrix, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // od razu robimy transponowanie
            invMatrix[i][j] = (inverseDet * invMatrix[i][j]) % 26;
        }
    }
    return invMatrix;
}

int keysGenerator(int size) {
    int inverseDet = 0;
//    int ** keyEnc; // RAZKOMMENTIT
    int ** keyDec;

    //to udalic
    int enc[3][3] = {15, 0, 18, 22, 22, 7, 14, 17, 11};
    int ** keyEnc = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        keyEnc[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) keyEnc[i][j] = enc[i][j];
    }
    // celikom

    do {
//         keyEnc = keyEncGenerator(size); //RAZKOMENTIT

         inverseDet = inverseDeterminant(keyEnc, size);
    }
    while (inverseDet == 0);
    printf_s("\n**** %d ****\n", inverseDet);

    //dalej znajdziemy macierz dopełnień algebraicznych
    keyDec = inverseMatrix(keyEnc, size, inverseDet);

    printf_s("\n############## keyEnc ###############\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf_s("+%d+", keyEnc[i][j]);
        }
        printf_s("\n");
    }
    printf_s("\n#######################################\n");

    printf_s("\n############## keyDec ###############\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf_s("+%d+", keyDec[i][j]);
        }
        printf_s("\n");
    }
    printf_s("\n#######################################\n");

}


int main() {
    int size;

    while (true) {
        scanf_s("%d", &size);
        keysGenerator(size);
    }


//    int input;
//    while( true ) {
//        scanf_s("%d", &input);
//
//
//        int ** out = keyEncGenerator(input);
//
//
//        printf_s("\n#######################################\n");
//        for (int i = 0; i < input; i++) {
//            for (int j = 0; j < input; j++) {
//                printf_s(" - %d - ", out[i][j]);
//            }
//            printf_s("\n");
//        }
//        printf_s("\n#######################################\n");
//        printf_s("\n############## WYZNACZNIK #############\n");
//        int deter = determinant(out, input);
//        printf_s("\n");
//        printf_s("wyzn: %d\n", deter);
//        inverseDeterminant(out, input);
//
//        free(out);
//    }


    system("pause");
	return 0;
}