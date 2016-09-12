#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//function prototypes
int getGraph(FILE *fp);
int checkLine(char *line);
int isValid(char *string);
int getIndex(char *name, int nodeCnt);
int appendNode(char *name, int nodeCnt);
void appendLink(int nodeCnt);
void sptree(int index, int nodeCnt);