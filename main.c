#include "cryptStruct.c"

// dwie podstawowe operacje
// opisujące szyfrowanie i deszyfrowanie tekstu
// zdefiniowany pod mainem
void encryptMessage();
void decryptMessage();

int main() {
  int operation;
  while(1) {
    // odczytywanie komandy z wiersza poleceń
    operation = menu();

    if (operation == 1) encryptMessage();
    if (operation == 2) decryptMessage();
    if (operation == 0) return 0; //exit
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

  msg.inputText = typeMessage(message); //tekst
  keys.size = typeKey(); //rozmiar klucza
  session.fileName = typeMessage(message2); //nazwa pliku
  // przekształcenie tekstu do przeliczeń
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
void decryptMessage() {
  struct Session session;
  struct Message msg = session.message;
  struct Keys keys = session.keys;
  // 0 - plik nie istnieje, 1 - OK, 2 - EXIT
  int isFileExist;

  // przydzielamy pamięć dla zmiennych
  msg.inputText = malloc(TEXTBUFFER * sizeof(char));
  keys.keyTextRep = malloc(TEXTBUFFER * sizeof(char));
  session.fileName = typeMessage("\nWprowadz nazwe pliku (np. text.txt) albo exit aby przerwac program: ");
  if (strcmp(session.fileName, "exit") == 0) return; // jezeli "exit" - wstrymamy program
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