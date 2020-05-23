#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>

int frobcmp(char const *a, char const *b)
{
  while (*a != ' ' && *b != ' ') {
    char clear1 = *a ^ 42;
    char clear2 = *b ^ 42;

    if (clear1 > clear2) {
      return 1;
    }
    else if (clear1 < clear2) {
      return -1;
    }
    a++;
    b++;
  }

  if (*a == ' ' && *b == ' ') {
    return 0;
  }
  if (*a == ' ') {
    return -1;
  }
  if (*b == ' ') {
    return 1;
  }
  return 0;
}

int noCaseCmp(char const *a, char const *b) {
  while (*a != ' ' && *b != ' ') {
    char clear1 = *a ^ 42;
    char clear2 = *b ^ 42;
    
    if (clear1 >= 0 && clear1 <= UCHAR_MAX) {
      char checkClear1 = toupper(clear1); //take a look
      clear1 = checkClear1;
    }

    if (clear2 >= 0 && clear2 <= UCHAR_MAX) {
      char checkClear2 = toupper(clear2); 
      clear2 = checkClear2;
    }

    if (clear1 > clear2) {
      return 1;
    }
    else if (clear1 < clear2) {
      return -1;
    }
    a++;
    b++;
  }

  if (*a == ' ' && *b == ' ') {
    return 0;
  }
  if (*a == ' ') {
    return -1;
  }
  if (*b == ' ') {
    return 1;
  }
  return 0;
}

int regCompare(const void* a, const void* b) {
  return frobcmp(*(char**) a, *(char**) b);
}

int noCaseCompare(const void* a, const void* b) {
  return noCaseCmp(*(char**) a, *(char**) b);
}

void allocError() {
  write(2, "Memory Allocation Error\n", 24);
  exit(1);
}

void inputError() {
  write(2, "Read Error\n", 11);
  exit(1);
}

void outputError() {
  write(2, "Output Error\n", 13);
}

int main(int argc, char* argv[]) { //methods of input?
  if (argc > 2) {
    write(2, "Too Many Arguments\n", 19);
    exit(1);
  }

  int ignoreCase = 0;
  if (argc == 2 && strcmp(argv[1], "-f") == 0) {
    ignoreCase = 1;
  }
 
  struct stat statusCheck;
  int canDetSize = fstat(0, &statusCheck);
  int size = 0;
  int file = 0;
  if (canDetSize >= 0) {
    if (S_ISREG(statusCheck.st_mode) != 0) {
      file = 1;
      size = statusCheck.st_size;
    }
  }
  
  int bytesRead = 0;

  char* buffer;
  if (!file || size == 0) {
    size = 8000; 
  }

  buffer = (char*)malloc(size);
  int readStatus = read(0, buffer, size);
  if (readStatus == -1) {
    inputError();
  }
  bytesRead = readStatus;

  //Reading further
  char next = 0;
  readStatus = read(0, &next, 1);
  if (readStatus == -1) {
    inputError();
  }

  while (readStatus) {
    if (bytesRead == size) {
      size = size * 2;
      buffer = (char*)realloc(buffer, size);
      if (buffer == NULL) {
	allocError();
      }
    }
    buffer[bytesRead] = next;
    bytesRead++;
    readStatus = read(0, &next, 1);
    if (readStatus == -1) {
      inputError();
    }
  }

  if (size > 0 && buffer[bytesRead - 1] != ' ') {
    buffer = (char*) realloc(buffer, sizeof(char)*(bytesRead + 1));
    if (buffer == NULL) {
      allocError();
    }
    buffer[bytesRead] = ' ';
    bytesRead++;
  }

  char **words = (char**)malloc(sizeof(char*));
  char *oneWord = (char*)malloc(sizeof(char));

  int c;
  int charPosition = 0;
  int rowPosition = 0;

  if (words == NULL || oneWord == NULL) {
    allocError();
  }

  int r = 0;
  for (r = 0; r < bytesRead; r++) {
    c = buffer[r];
    oneWord[charPosition] = c;
    if (c == ' '){
      words[rowPosition] = oneWord;
      rowPosition++;
      words = (char**)realloc(words, sizeof(char*)*(rowPosition + 1));
      if (words == NULL) {
	allocError();
      }
      char* swap = (char*)malloc(sizeof(char));
      oneWord = swap;
      if (oneWord == NULL) {
	allocError();
      }
      charPosition = 0;
    }
    else {
      charPosition++;
      oneWord = (char*)realloc(oneWord, sizeof(char)*(charPosition+1));
      if (oneWord == NULL) {
	allocError();
      }
    }
  }

  if (ignoreCase) {
    qsort(words, rowPosition, sizeof(char*), noCaseCompare);
  }
  else {
    qsort(words, rowPosition, sizeof(char*), regCompare);
  }

  int errorStatus = 1;
  char* temp;
  int k = 0;
  for (k = 0; k < rowPosition; k++) {
    temp = words[k];
    int j = 0;
    while (temp[j] != ' ') {
      j++;
    }
    errorStatus = write(1, words[k], ++j);
    if (errorStatus == -1) {
      outputError();
    }
  }

  int l = 0;
  for (l = 0; l < rowPosition; l++) {
    free(words[l]);
  }
  free(words);
  free(oneWord);
  free(buffer);
  exit(0);
  //do a leak check, there's still reachable memory, nullptr?
}
