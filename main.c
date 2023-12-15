#include <cscapi.h> //pause function
#include "functions.h"
#include <stdbool.h>


int inverseDeterminant (int **matrix, int size) {

    // unikamy ułamków poprzez użycie odwrotności wyznacznika na mod26
    // trzeba dla dezsyfrowania
    int inverseDet;
    for (int i = 0; i < alphLength; i++) {
        int det = (abs(determinant(matrix, size)) * i) % 26;
        if ( det == 1) {
            inverseDet = i;
        }
    }
    printf_s("invDet: %d\n", inverseDet);
    return inverseDet;
}


int main() {

    int input;
    while( true ) {
        scanf_s("%d", &input);


        int ** out = keyEncGenerator(input);


        printf_s("\n#######################################\n");
        for (int i = 0; i < input; i++) {
            for (int j = 0; j < input; j++) {
                printf_s(" - %d - ", out[i][j]);
            }
            printf_s("\n");
        }
        printf_s("\n#######################################\n");
        printf_s("\n############## WYZNACZNIK #############\n");
        int deter = determinant(out, input);
        printf_s("\n");
        printf_s("wyzn: %d\n", deter);
        inverseDeterminant(out, input);

        free(out);

    }


    system("pause");
	return 0;
}