#include "sim.h"
#include "sim_utils.h"

int getGraph(FILE *fp)
{
   	char line[MAX_LINE];
   	char* inp;
	int type;
	char nodeA[MAX_IDENT];
	char nodeB[MAX_IDENT];
	char graphName[MAX_IDENT];
	int number;
	int nodeCnt = 0;

    while((inp = fgets(line, MAX_LINE, fp)))
    {
		type = checkLine(line);
		switch(type)
		{
			//new node
			case 0:
				//printf("DEBUG(line):%s\n", line);
				sscanf(line, "%s = %d", nodeA, &number);
				//printf("DEBUG(name): %s\n", nodeA);
				//printf("DEBUG(number): %d\n", number);
				if(isValid(nodeA) == 1) return 1;//printf("Name not valid!\n");
				//else printf("Name is valid!\n");
				break;
			//new link
			case -1:
				//printf("DEBUG(line):%s\n", line);
				sscanf(line, "%s - %s : %d", nodeA, nodeB, &number);
				//printf("DEBUG(nameA): %s\n", nodeA);
				//printf("DEBUG(nameB): %s\n", nodeB);
				//printf("DEBUG(number): %d\n", number);
				if(isValid(nodeA) == 1) return 1;//printf("Name not valid!\n");
				//else printf("Name is valid!\n");
				if(isValid(nodeB) == 1) return 1;//printf("Name not valid!\n");
				//else printf("Name is valid!\n");
				break;
			//begin line
			case -2:
				//printf("DEBUG(line):%s\n", line);
				/*
					Not really elegant, but it works
					didn't know how to ignore the first string "Graph" but it isn't needed so you can scan and then override it
				*/
				sscanf(line, "%s %s", graphName, graphName);
				//printf("DEBUG(name): %s\n", graphName);
				break;
			//end line
			case -3:
				return 0;
				break;
		}
		//getchar();
    }
}

linetype checkLine(char *line)
{
	//Trim leading space
	while(isspace(*line)) line++;

	//return 1 if line is commentline or blank
	if(*line == 0 || strncmp(line, "//", 2) == 0)  return typeCommand_blank;

	//if it is a new node definition
	if(strchr(line, '=') != 0) return typeNode;
	//if it is a new link definition
	else if(strchr(line, ':') != 0) return typeLink;
	//begin of graph definition
	else if(strchr(line, '{') != 0) return typeGraph_begin;
	//end of graph definition
	else if(strchr(line, '}') != 0) return typeGraph_end;
}

int isValid(char *string)
{
	int count = 0;

	//printf("DEBUG(char,alpha,digit): %c %d %d\n", *string, isalpha(*string), isdigit(*string));
	if(isalpha(*string) == 0) return 1;
	count++;
	string++;

	while(count <= MAX_IDENT)
	{
		//printf("DEBUG(char,alpha,digit): %c %d %d\n", *string, isalpha(*string), isdigit(*string));
		//if name has ended
		if(*string == '\0') return 0;
		//if name contains illegal char
		if(isalpha(*string) == 0 && isdigit(*string) == 0) return 1;
		count++;
		string++;
	}
	return 1;
}

int getIndex(char *name, int nodeCnt)
{

}

int appendNode(char *name, int nodeCnt)
{
	
}

void appendLink(int nodeCnt)
{
	
}

void sptree(int index, int nodeCnt)
{
	
}
