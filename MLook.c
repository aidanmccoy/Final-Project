#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 1024


int LineCount(char *filename) {
   FILE *fp = fopen(filename, "r");
   int lines = 0;
   char currChar;

   while (!feof(fp)) {
      currChar = fgetc(fp);
      if (currChar == '\n') {
         lines++;
      }
   }
   fclose(fp);
   return lines;
}

void RemoveNL(char *str) {
   while (*str != '\n') {
      str++;
   }
   *str = '\0';
}

FILE *ReadFile(char *argv[], int argc, char *target,
 char *termChar, int *lineCount, int *dFlag, int *fFlag, int *tFlag)  {
   int argsIter;
   FILE *returnFile;

   if (argv[1] == NULL || *argv[1] == '-') {
      exit(1);
   }
   else {
      strcpy(target, argv[1]);
   }

   if (argv[2] == NULL || *argv[2] == '-') {
      returnFile = fopen("/usr/share/dict/words", "r");
      *lineCount = LineCount("/usr/share/dict/words");
      *dFlag = 1;
      *fFlag = 1;
   }
   else {
      returnFile = fopen(argv[2], "r");
      *lineCount = LineCount(argv[2]);
   }

   for (argsIter = 0; argsIter < argc; argsIter++) {
      if (*argv[argsIter] == '-') {
         if (*(argv[argsIter] + 1) == 'f') {
            *fFlag = 1;
            if(*(argv[argsIter] + 2) == 'd') {
               *dFlag = 1;
            }
            *fFlag = 1;
         }
         if (*(argv[argsIter] + 1) == 'd') {
            *dFlag = 1;
            if(*(argv[argsIter] + 2) == 'f') {
               *fFlag = 1;
            }
         }
         if (*(argv[argsIter] + 1) == 't') {
            *tFlag = 1;
            strcpy(termChar, argv[++argsIter]);
         }
      }
   }
   return returnFile;
}

char *Flags(char *target, char *str, int fFlag, int tFlag,
 int dFlag, char *termChar) {
   char *returnStr = calloc(MAX_SIZE, 1);
   int len = strlen(str);
   int strIter;
   int returnIter = 0;
   int tFlagLen;
    
   len = strlen(target);
   if (tFlag) {
      for (tFlagLen = 0; tFlagLen < strlen(target); tFlagLen++) {
         if (target[tFlagLen] == *termChar) {
            break;
         }
      }
      len = tFlagLen;
   }


   if (dFlag) {
      for (strIter = 0; strIter < len + 1; strIter++) {
         if (isalnum(*(str + strIter))) {
            memcpy((returnStr + returnIter), (str + strIter), 1);
            returnIter++;
         }
      }
   }
   else {
      for (strIter = 0; strIter < len + 1; strIter++) {
         memcpy((returnStr + strIter), str + strIter, 1);
      }
   }

   if (fFlag) {
      for (returnIter = 0; returnStr[returnIter]; returnIter++) {
         returnStr[returnIter] = tolower(returnStr[returnIter]);
      }
   }
   return returnStr;

}

int BinSort(char *lines[], int lineCount, char *target,
 int fFlag, int tFlag, int dFlag, char *termChar) {
   int high;
   int low;
   int mid = lineCount / 2;

   char *toCompare;

   toCompare = Flags(target, lines[lineCount / 2], fFlag,
    tFlag, dFlag, termChar);

   if (strncmp(target, toCompare, strlen(toCompare)) < 0) {
      high = lineCount / 2;
      low = 0;
   }
   else if (strncmp(target, toCompare, strlen(toCompare)) > 0) {
      high = lineCount;
      low = lineCount / 2;
   }
   else {
      return lineCount / 2;
   }   

   while (high != low && (high - low) != 1) {
      mid = low + ((high - low) / 2);
      toCompare = Flags(target, lines[mid], fFlag, tFlag, dFlag, termChar);

      if (strncmp(target, toCompare, strlen(toCompare)) < 0) {
         high = mid;
      }
      else if (strncmp(target, toCompare, strlen(toCompare)) > 0) {

         low = mid;
      }
      else {
         break;
      }
   }
   return mid;
}

void FillLines(char *lines[], FILE *file, int lineCount) {
   int lineIter;
   char *temp = calloc(MAX_SIZE, 1);

   for (lineIter = 0; lineIter < lineCount; lineIter++) {
      temp = fgets(temp, MAX_SIZE - 1, file);
      RemoveNL(temp);
      strcpy(lines[lineIter], temp);
   }
}

void Print(char *lines[], int targetNDX, char *target,
 int fFlag, int tFlag, int dFlag, char *termChar) {
   int iter = targetNDX;
   char *toCompare;
   int compLen;

   toCompare = Flags(target, lines[iter], fFlag, tFlag, dFlag, termChar);
   while (!strncmp(toCompare, target, strlen(toCompare))) {
      iter--;
      toCompare = Flags(target, lines[iter], fFlag, tFlag, dFlag, termChar);

   }
   iter++;
   toCompare = Flags(target, lines[iter], fFlag, tFlag, dFlag, termChar);
   if (strlen(target) < strlen(toCompare)) {
      compLen = strlen(target);
   }
   else {
      compLen = strlen(toCompare);
   }

   while (!strncmp(target, toCompare, compLen)) {

      printf("%s\n", lines[iter]);
      iter++;
      toCompare = Flags(target, lines[iter], fFlag, tFlag, dFlag, termChar);
      if (strlen(target) < strlen(toCompare)) {
         compLen = strlen(target);
      }
      else {
         compLen = strlen(toCompare);
      }
   }
}


int main(int argc, char *argv[]) {
   int lineCount;
   int dFlag = 0;
   int fFlag = 0;
   int tFlag = 0;
   int iter;
   int targetFound;
   char *target = calloc(MAX_SIZE, 1);
   char **lines;
   char *termChar = calloc(MAX_SIZE, 1);


   FILE *file;

   file = ReadFile(argv, argc, target, termChar, &lineCount,
    &dFlag, &fFlag, &tFlag);

   if (fFlag) {
      for (iter = 0; target[iter]; iter++) {
         target[iter] = tolower(target[iter]);
      }
   }

   lines = calloc(sizeof(int *), lineCount);
   for (iter = 0; iter < lineCount; iter++) {
      lines[iter] = calloc(MAX_SIZE, 1);
   }

   FillLines(lines, file, lineCount);

   fclose(file);

   targetFound = BinSort(lines, lineCount, target,
    fFlag, tFlag, dFlag, termChar);

   Print(lines, targetFound, target, fFlag, tFlag, dFlag, termChar);

   return 0;
}