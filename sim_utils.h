#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef enum linetype { typeNode, typeCommand_blank, typeLink, typeGraph_begin, typeGraph_end } linetype;

//function prototypes
int getGraph(FILE *fp);
linetype checkLine(char *line);
int isValid(char *string);
int getIndex(char *name, int nodeCnt);
int appendNode(char *name, int nodeCnt);
void appendLink(int nodeCnt);
void sptree(int index, int nodeCnt);