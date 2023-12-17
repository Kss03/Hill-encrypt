#include <cscapi.h> //pause function
#include "functions.h"
#include <stdbool.h>

int main() {
    int size;

    while (true) {
        scanf_s("%d", &size);

        keysGenerator(size);


    }

    system("pause");
	return 0;
}