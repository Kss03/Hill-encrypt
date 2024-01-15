#include <cscapi.h> //pause function
#include "functions.c"
#include <stdbool.h>
#include <string.h>




struct TextKeys keysGenerator(struct SessionData data) {
    int inverseDet = 0;

    do {
        data.keys.encryptKey = keyEncGenerator(data.keys.size);
        inverseDet = inverseDeterminant(data.keys.encryptKey, data.keys.size);
    }
    while (inverseDet == 0);
    printf_s("\n****  %d ****\n", inverseDet);

    //dalej znajdziemy macierz odwrotną
    data.keys.decryptKey = inverseMatrix(data.keys.encryptKey, data.keys.size, inverseDet);

    printf_s("\n############## keyEnc ###############\n");
    for (int i = 0; i < data.keys.size; i++) {
        for (int j = 0; j < data.keys.size; j++) {
            printf_s("+%d+", data.keys.encryptKey[i][j]);
        }
        printf_s("\n");
    }
    printf_s("\n#######################################\n");

    printf_s("\n############## keyDec ###############\n");
    for (int i = 0; i < data.keys.size; i++) {
        for (int j = 0; j < data.keys.size; j++) {
            printf_s("+%d+", data.keys.decryptKey[i][j]);
        }
        printf_s("\n");
    }
    printf_s("\n#######################################\n");

    return data.keys;
}


int main() {
    struct SessionData data;

    while (true) {
//        printf_s("\nText: ");
//        ("%s", &data.text.text);
//        data.text.length = strlen(data.text.text);

//
//        printf_s("\ntext length: %d", strlen(inputText.text));
//        printf_s("\ntext out: %s", inputText.text);

        printf_s("\nWprowadz text (litery Aa-Zz):");
        fgets(data.inputData.text, TEXTBUFFER, stdin);
        fputs(data.inputData.text, stdout);
        printf_s("\n%s", data.inputData.text);
        printf_s("%s", data.inputData.text);
        puts("*****************************");

        printf_s("\nWymiar macierzy dla klucza (od 3 do 10): ");
        scanf("%d", &data.keys.size);

        data.keys = keysGenerator(data);

        printf_s("%s", data.inputData.text);
        data.inputData.length = strlen(data.inputData.text) - 1;
        printf_s("\n - - - %d - - - -\n", data.inputData.length);

        // przy inicjalizacji lancuchu dopasowywujemy go do wymiary macierzy
        // czyli wypelniamy lancuch zeby byl wielokrotny rozmiaru klucza
        int textOnNumLength;
        if (data.inputData.length % data.keys.size == 0) textOnNumLength = data.inputData.length;
        else textOnNumLength = data.inputData.length / data.keys.size * data.keys.size + data.keys.size;

        data.textOnNum = (int *)malloc(textOnNumLength * sizeof(int));
        for (int i = 0; i < textOnNumLength; i++) {
            if (data.inputData.text[i]) {
                data.textOnNum[i] = letToNum(data.inputData.text[i]);
                printf_s("%d", data.textOnNum[i]);
            } else {
                data.textOnNum[i] = 0;
                printf_s("%d", data.textOnNum[i]);
            }
        }

        int count = ceil(((double)textOnNumLength / data.keys.size));
        int letterNum = 0;
        int arr[data.keys.size];
        printf_s("\n############## TEXT ENCRYPT ###############\n");
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < data.keys.size; j++) {
                for(int x = 0; x < data.keys.size; x++) {
//                    printf_s("** %d --%d", i * data.keys.size + x, data.keys.encryptKey[j][x]);
                    letterNum = letterNum + (data.textOnNum[i * data.keys.size + x] * data.keys.encryptKey[j][x]);
                }
                arr[j] = letterNum % 26;
                letterNum = 0;

            }
            for (int j = 0; j < data.keys.size; j++) {
                data.textOnNum[i * data.keys.size + j] = arr[j];
                printf_s("--%d", data.textOnNum[i * data.keys.size + j]);
            }
            printf_s("\n");
        }

        printf_s("\n############## TEXT DECRYPT ###############\n");
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < data.keys.size; j++) {
                for(int x = 0; x < data.keys.size; x++) {
//                    printf_s("** %d --%d", i * data.keys.size + x, data.keys.decryptKey[j][x]);
                    letterNum = letterNum + (data.textOnNum[i * data.keys.size + x] * data.keys.decryptKey[j][x]);
                }
                arr[j] = letterNum % 26;
                letterNum = 0;

            }
            for (int j = 0; j < data.keys.size; j++) {
                data.textOnNum[i * data.keys.size + j] = arr[j];
                printf_s("--%d", data.textOnNum[i * data.keys.size + j]);
            }
            printf_s("\n");
        }

        printf_s("END\n");
        data.decodedText = (char *)malloc(textOnNumLength * sizeof(char));
        for (int i = 0; i < textOnNumLength; i++) {
            if (data.textOnNum[i] || data.textOnNum[i] == 0) {
                printf_s("\n%d -", data.textOnNum[i]);
                data.decodedText[i] = numToLet(data.textOnNum[i]);
                printf_s(" %c ", data.decodedText[i]);
            }
        }
    }

    free(data.keys.encryptKey);
    free(data.keys.decryptKey);

    system("pause");
	return 0;
}