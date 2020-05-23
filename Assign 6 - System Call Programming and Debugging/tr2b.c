#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int returnIndex(char* temp, char c) {
  char* track = temp;
  int position = -1;
  int positionTrack = 0;
  while (*track != '\0') {
    if (*track == c) {
      position = positionTrack;
      break;
    }
    positionTrack++;
    track++;
  }
  return position;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Input should only be 2 arguments\n");
    exit(1);
  }
  char *from = argv[1];
  char *to = argv[2];
  
  if (strlen(from) != strlen(to)) {
    fprintf(stderr, "Inputs should have the same length\n");
    exit(1);
  }

  char* temp1 = from;
  char* temp2 = temp1 + 1;
  while (*temp1 != '\0') {
    while (*temp2 != '\0') {
      if (*temp1 == *temp2) {
	fprintf(stderr, "First input should not have duplicate bytes\n");
	exit(1);
      }
      temp2++;
    }
    temp1++;
    temp2 = temp1 + 1;
  }
  
  char current = ' ';
  int thisPos = -1;
  while ((current = getchar()) != EOF) {
    thisPos = returnIndex(from, current);
    if (thisPos == -1) {
      putchar(current);
    }
    else {
      putchar(to[thisPos]);
    }
  }
  return 0;
}
