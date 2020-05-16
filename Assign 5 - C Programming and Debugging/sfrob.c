#include <stdio.h>
#include <stdlib.h>

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

  //Check for empty input     
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

int stageCompare(const void* a, const void* b) {
  return frobcmp(*(char**) a, *(char**) b);
}

int main() {
  char **words = (char**)malloc(sizeof(char*));
  char *oneWord = (char*)malloc(sizeof(char));
  if (words == NULL || oneWord == NULL) {
    fprintf(stderr, "Memory Allocation Error");
    exit(1);
  }

  int c;
  int prev;
  int charPosition = 0;
  int rowPosition = 0;
    
  while (c != EOF) {
    c = getchar();
    if (ferror(stdin) != 0) {
      fprintf(stderr, "Input Error");
      exit(1);
    }
    if (c == EOF) {
      if (prev != ' ') {
	oneWord[charPosition] = ' ';
	words[rowPosition] = oneWord;
        rowPosition++;
      }
      else {
	oneWord = (char*)realloc(oneWord, sizeof(char)*(charPosition + 1));
	if (oneWord == NULL) {
	  fprintf(stderr, "Reallocation Error");
	  // exit(1);
	}
      }
      break;
    }
    else {
      oneWord[charPosition] = c;
    }

    if (c == ' '){
	words[rowPosition] = oneWord;
	rowPosition++;
	words = (char**)realloc(words, sizeof(char*)*(rowPosition + 1));
	if (words == NULL) {
	  fprintf(stderr, "Reallocation Error");
	  exit(1);
	}
	char* swap = (char*)malloc(sizeof(char));
	oneWord = swap;
	if (oneWord == NULL) {
	  fprintf(stderr, "Allocation Error");
	  exit(1);
	}
	charPosition = 0;
    }
    else {
      charPosition++;
      oneWord = (char*)realloc(oneWord, sizeof(char)*(charPosition+1));
      if (oneWord == NULL) {
	fprintf(stderr, "Reallocation Error");
	exit(1);
      }
    }
    prev = c;

  }
  // printf("%s\n", words[howmany]);
  qsort(words, rowPosition, sizeof(char*), stageCompare);
  char* temp;
  for (int i = 0; i < rowPosition; i++) { //is initial declaration okay
    temp = words[i];
    int j = 0;
    while (temp[j] != ' ') {
      putchar(temp[j]);
      if (ferror(stdout) != 0) {
	fprintf(stderr, "Output Error");
	exit(1);
      }
      j++;
    }
    putchar(' ');
    if (ferror(stdout) != 0) {
      fprintf(stderr,"Output Error");
      exit(1);
    }
  }
  
  for (int i = 0; i < rowPosition; i++) {
    free(words[i]);
  }
  free(words);
  exit(0);
}
