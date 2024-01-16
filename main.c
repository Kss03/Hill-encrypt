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

struct SessionData initializationVariables (struct SessionData data) {
    // długosc wpisanego tekstu
    data.inputData.length = strlen(data.inputData.text) - 1;

    // przy inicjalizacji lancuchu dopasowywujemy go do wymiary macierzy
    // czyli wypelniamy lancuch zeby byl wielokrotny rozmiaru klucza
    if (data.inputData.length % data.keys.size == 0) data.textOnNumLength = data.inputData.length;
    else data.textOnNumLength = data.inputData.length / data.keys.size * data.keys.size + data.keys.size;
    printf_s("\ntextOnNumLen: %d \n", data.textOnNumLength);
    return data;
}

struct SessionData convertTextToNum(struct SessionData data) {
    data.textOnNum = (int *)malloc(data.textOnNumLength * sizeof(int));
    for (int i = 0; i < data.textOnNumLength; i++) {
        if (data.inputData.text[i]) {
            data.textOnNum[i] = letToNum(data.inputData.text[i]);
//            printf_s("%d", data.textOnNum[i]);
        } else {
            data.textOnNum[i] = ALPHLENGTH - 1;
//            printf_s("%d", data.textOnNum[i]);
        }
    }
    return data;
}

char *convertNumToText(struct SessionData data) {
    printf_s("END\n");
    char *text = malloc(data.textOnNumLength * sizeof(char));
    for (int i = 0; i < data.textOnNumLength; i++) {
        if (data.textOnNum[i] || data.textOnNum[i] == 0) {
//            printf_s("\n%d -", data.textOnNum[i]);
            text[i] = numToLet(data.textOnNum[i]);
            printf_s(" %c ", text[i]);
        }
    }
    printf_s("textttttttt: %s\n", text);
    return text;
}

struct SessionData convertKeyToText(struct SessionData data) {
    int size = data.keys.size;
    char *text = malloc(data.textOnNumLength * sizeof(char));
    data.keys.encryptKeyText = malloc( size * size * sizeof(char));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            text[i * size + j] = numToLet(data.keys.encryptKey[i][j]);
            printf_s("-%c-", text[i * size + j]);
        }
    }
    data.keys.encryptKeyText = text;
    return data;
}

struct SessionData encryptText(struct SessionData data) {
    int count = ceil(((double)data.textOnNumLength / data.keys.size));
    int letterNum = 0;
    int arr[data.keys.size];
    printf_s("\n############## TEXT ENCRYPT ###############\n");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < data.keys.size; j++) {
            for(int x = 0; x < data.keys.size; x++) {
//                    printf_s("** %d --%d", i * data.keys.size + x, data.keys.encryptKey[j][x]);
                letterNum = letterNum + (data.textOnNum[i * data.keys.size + x] * data.keys.encryptKey[j][x]);
            }
            arr[j] = letterNum % ALPHLENGTH;
            letterNum = 0;

        }
        for (int j = 0; j < data.keys.size; j++) {
            data.textOnNum[i * data.keys.size + j] = arr[j];
//            printf_s("--%d", data.textOnNum[i * data.keys.size + j]);
        }
//        printf_s("\n");
    }
    return data;
}

struct SessionData decryptText(struct SessionData data) {
    int count = ceil(((double)data.textOnNumLength / data.keys.size));
    int letterNum = 0;
    int arr[data.keys.size];
    printf_s("\n############## TEXT DECRYPT ###############\n");
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < data.keys.size; j++) {
            for(int x = 0; x < data.keys.size; x++) {
//                    printf_s("** %d --%d", i * data.keys.size + x, data.keys.decryptKey[j][x]);
                letterNum = letterNum + (data.textOnNum[i * data.keys.size + x] * data.keys.decryptKey[j][x]);
            }
            arr[j] = letterNum % ALPHLENGTH;
            letterNum = 0;

        }
        for (int j = 0; j < data.keys.size; j++) {
            data.textOnNum[i * data.keys.size + j] = arr[j];
//            printf_s("--%d", data.textOnNum[i * data.keys.size + j]);
        }
//        printf_s("\n");
    }
}

void toSaveTextAndKey(struct SessionData data) {
    //savedatas
    puts("\n*****************************");
    printf_s("text: %s", data.codedText);
    printf_s("\nkey: %s", data.keys.encryptKeyText);
    puts("\n*****************************");
    int let;
    FILE *file = fopen("encrypt-and-key.txt", "w");
    for (int i = 0; i < data.textOnNumLength; i++) {
        let = data.codedText[i];
        fprintf_s(file, &let);
    }

    fprintf_s(file, "\n");
    for (int i = 0; i < data.keys.size * data.keys.size; i++) {
        let = data.keys.encryptKeyText[i];
        fprintf_s(file, &let);
    }
//    fprintf_s(file, data.keys.encryptKeyText);
    fclose(file);
}

struct SessionData toReadCoddedTextAndKey(struct SessionData data) {
    char line[255];
    FILE *file = fopen("encrypt-and-key.txt", "r");
    fgets(data.codedText, 255, file);
    printf_s("\nline1: %s", data.codedText);
    fgets(data.keys.encryptKeyText, 255, file);
    printf_s("\nline2: %s", data.keys.encryptKeyText);
    fclose(file);
    return data;
}

int main() {
//    setlocale(LC_ALL, "en_US");
    struct SessionData data;

    printf_s("\nWprowadz text (litery Aa-Zz):");
    fgets(data.inputData.text, TEXTBUFFER, stdin);
    fputs(data.inputData.text, stdout);
    puts("*****************************");
    printf_s("\nWymiar macierzy dla klucza (od 3 do 10): ");
    scanf_s("%d", &data.keys.size);

    data.keys = keysGenerator(data);
    data = convertKeyToText(data);

    data = initializationVariables(data);
    printf_s("%s", data.inputData.text);

    data = convertTextToNum(data);

    data = encryptText(data);
    data.codedText = convertNumToText(data);
    printf_s("%s\n", data.codedText);

    toSaveTextAndKey(data);

    data = toReadCoddedTextAndKey(data);
    printf_s("\nout1: %s", data.codedText);
    printf_s("\nout2: %s", data.keys.encryptKeyText);

    data = decryptText(data);
    data.decodedText = convertNumToText(data);
    puts("\n*****************************");
    printf_s("text: %s", data.decodedText);
    puts("\n*****************************");

    system("pause");

    free(data.keys.encryptKey);
    free(data.keys.decryptKey);
    free(data.keys.encryptKeyText);
    free(data.decodedText);
    free(data.textOnNum);

    system("pause");
	return 0;
}