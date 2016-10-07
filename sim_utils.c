#include "sim.h"
#include "sim_utils.h"

int getGraph(FILE *fp, list *list)
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
            case typeNode:
                sscanf(line, "%s = %d", nodeA, &number);
#ifdef DEBUG
                printf("DEBUG(line):%s\n", line);
                printf("DEBUG(name): %s\n", nodeA);
				printf("DEBUG(number): %d\n", number);
#endif
                if(isValid(nodeA))
                {
                    nodeCnt = appendNode(list, nodeA, nodeCnt);
                }
				break;
			//new link
			case typeLink:
                sscanf(line, "%s - %s : %d", nodeA, nodeB, &number);
#ifdef DEBUG
                printf("DEBUG(line):%s\n", line);
				printf("DEBUG(nameA): %s\n", nodeA);
				printf("DEBUG(nameB): %s\n", nodeB);
                printf("DEBUG(number): %d\n", number);
#endif

                if(isValid(nodeA) && isValid(nodeB))
                {
                    nodeCnt = appendNode(list, nodeA, nodeCnt);
                    nodeCnt = appendNode(list, nodeB, nodeCnt);

                    //TODO exist link a->b? create
                }
                break;
			//begin line
			case typeGraph_begin:
				//printf("DEBUG(line):%s\n", line);
				/*
					Not really elegant, but it works
					didn't know how to ignore the first string "Graph" but it isn't needed so you can scan and then override it
				*/
				sscanf(line, "%s %s", graphName, graphName);
				//printf("DEBUG(name): %s\n", graphName);
				break;
			//end line
			case typeGraph_end:
				return nodeCnt;
		}
    }

    //TODO if nodeCnt != 0 -> free nodeList
    return 0;
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

    return typeUndefined;
}

bool isValid(char *string)
{
	int count = 0;

	//printf("DEBUG(char,alpha,digit): %c %d %d\n", *string, isalpha(*string), isdigit(*string));
	if(isalpha(*string) == 0) return false;
	count++;
	string++;

	while(count <= MAX_IDENT)
	{
		//printf("DEBUG(char,alpha,digit): %c %d %d\n", *string, isalpha(*string), isdigit(*string));
		//if name has ended
		if(*string == '\0') return true;
		//if name contains illegal char
		if(isalpha(*string) == 0 && isdigit(*string) == 0) return false;
		count++;
		string++;
	}
	return false; //this should be never reached
}

int getIndex(char *name, int nodeCnt)
{
    return 1;
}

int appendNode(list *nodelist, char *name, int nodeCnt)
{
    if(!list_exist(nodelist, name, compare_node_name)) {
#ifdef DEBUG
        printf("DEBUG(Append new node): %s (gesamt: %d)\n", name, nodeCnt+1);
#endif
        node *newnode = malloc(sizeof(node));
        strcpy(newnode->name, name);
        newnode->nodeID = atoi(name);

        list_append(nodelist, newnode);

        if (newnode->plinks != NULL) {
            free_node(newnode);
        }
        free(newnode);

        nodeCnt++;
    }

    return nodeCnt;
}

void appendLink(int nodeCnt)
{

}

void sptree(int index, int nodeCnt)
{

}
