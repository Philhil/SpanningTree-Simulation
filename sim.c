#include "sim.h"

node *pnode = NULL;
char graphname[MAX_IDENT];

int getGraph(FILE *fp)
{
   	char line[MAX_LINE];
   	char* inp;
	int type;
	char nodeA[MAX_IDENT];
	char nodeB[MAX_IDENT];
	char graphName[MAX_IDENT];
	int number, indexA, indexB;
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
                printf("\nDEBUG(line):%s\n", line);
                printf("DEBUG(name): %s\n", nodeA);
				printf("DEBUG(number): %d\n", number);
				#endif
				if(number < 0 || number > MAX_NODE_ID)
				{
					printf("Ungültiges Knotengewicht!");
					return 0;
				}
				
                if(isValid(nodeA))
                {
					if((indexA = getIndex(nodeA, nodeCnt)) != -1)
					{
						//Knoten schon vorhanden, aber keine ID definiert
						if(pnode[indexA].nodeID == NODE_ID_INIT)
						{
							pnode[indexA].nodeID = number;
                            pnode[indexA].rootID = number;
                            pnode[indexA].nextHop = number;
							#ifdef DEBUG
							printf("DEBUG: id %d appended to node %s\n", number, pnode[indexA].name);
							#endif
						}
					}
					else
					{
                    	nodeCnt = appendNode(nodeA, nodeCnt);
						if(nodeCnt == 0) return 0;
						pnode[nodeCnt-1].nodeID = number;
                        pnode[nodeCnt-1].rootID = number;
                        pnode[nodeCnt-1].nextHop = number;
						#ifdef DEBUG
						printf("DEBUG: node %s id %d appended\n", nodeA, number);
						#endif
					}
				}
				else
				{
					printf("Ungültiger Knoten!\n");
					return 0;
				}
				break;
			//new link
			case typeLink:
                sscanf(line, "%s - %s : %d", nodeA, nodeB, &number);
				#ifdef DEBUG
                printf("\nDEBUG(line):%s\n", line);
				printf("DEBUG(nameA): %s\n", nodeA);
				printf("DEBUG(nameB): %s\n", nodeB);
                printf("DEBUG(number): %d\n", number);
				#endif
				if(number < 0 || number > MAX_COSTS)
				{
					printf("Ungültiges Kantengewicht!");
					return 0;
				}

                if(isValid(nodeA) && isValid(nodeB))
                {
					if(getIndex(nodeA, nodeCnt) == -1)
					{
                    	nodeCnt = appendNode(nodeA, nodeCnt);
						if(nodeCnt == 0) return 0;
						#ifdef DEBUG
						printf("DEBUG: node %s id %d appended\n", nodeA, NODE_ID_INIT);
						#endif
					}
					if(getIndex(nodeB, nodeCnt) == -1)
					{
                    	nodeCnt = appendNode(nodeB, nodeCnt);
						if(nodeCnt == 0) return 0;
						#ifdef DEBUG
						printf("DEBUG: node %s id %d appended\n", nodeB, NODE_ID_INIT);
						#endif
					}
					
                 	indexA = getIndex(nodeA, nodeCnt);
                 	indexB = getIndex(nodeB, nodeCnt);
                 	
                 	if(pnode[indexA].plinks[indexB].costs != 0 || pnode[indexB].plinks[indexA].costs != 0)
					{
						printf("Kante doppelt definiert!\n");
						return 0;
					}
					
					//set costs to links
					pnode[indexA].plinks[indexB].costs = number;
                 	pnode[indexB].plinks[indexA].costs = number;
                }
				else
				{
					printf("Ungültige Knoten!\n");
					return 0;
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
				strcpy(graphname, graphName);
				#ifdef DEBUG
				printf("DEBUG(name): %s\n", graphname);
				#endif
				break;
			//end line
			case typeGraph_end:
				return nodeCnt;
		}
    }

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

int isValid(char *string)
{
	int count = 0;

	//printf("DEBUG(char,alpha,digit): %c %d %d\n", *string, isalpha(*string), isdigit(*string));
	if(isalpha(*string) == 0) return 0;
	count++;
	string++;

	while(count <= MAX_IDENT)
	{
		//printf("DEBUG(char,alpha,digit): %c %d %d\n", *string, isalpha(*string), isdigit(*string));
		//if name has ended
		if(*string == '\0') return 1;
		//if name contains illegal char
		if(isalpha(*string) == 0 && isdigit(*string) == 0) return 0;
		count++;
		string++;
	}
	return 0; //this should be never reached
}

int getIndex(char *name, int nodeCnt)
{
	for(int i = 0; i < nodeCnt; i++)
	{
		if(strcmp(pnode[i].name, name) == 0)
			return i;
	}
	
    return -1;
}

int appendNode(char *name, int nodeCnt)
{
	node *tmp;
    nodeCnt++;

	tmp = (node*) realloc(pnode, nodeCnt*sizeof(node));
	if(tmp == NULL)
	{
		//error in realloc
		printf("Fehler beim realloc der Nodeliste!\n");
		free(pnode);
		return 0;
	}
	pnode = tmp;

	strcpy(pnode[nodeCnt-1].name, name);
	
	//set NodeID to a value that is incorrect to check if it was defined or not
	pnode[nodeCnt-1].nodeID = NODE_ID_INIT;
    pnode[nodeCnt-1].rootID = NODE_ID_INIT;
    pnode[nodeCnt-1].nextHop = NODE_ID_INIT;
	
	appendLink(nodeCnt);
	
	return nodeCnt;
}

void appendLink(int nodeCnt)
{
	link* tmp;
	
	pnode[nodeCnt-1].plinks = NULL;
	
	//realloc all linklists so they have the same length as there are nodes
	for(int i = 0; i < nodeCnt; i++)
	{
		tmp = (link*) realloc(pnode[i].plinks, nodeCnt*sizeof(link));
		if(tmp == NULL)
		{
			//error in realloc
			printf("Fehler beim realloc der Linkliste!\n");
			free(pnode[i].plinks);
			return;
		}
		pnode[i].plinks = tmp;
	}
	
	//put all new links to 0 so they don't exist
	for(int i = 0; i < nodeCnt; i++)
	{
		pnode[i].plinks[nodeCnt-1].costs = 0;
		if(i == nodeCnt-1)
		{
			for(int j = 0; j < nodeCnt; j++)
			{
				pnode[i].plinks[j].costs = 0;
			}
		}
	}
}

int getNeighbourRoot(int index)
{
    return pnode[index].rootID;
}

int getNeighbourCoststoRoot(int index)
{
    return pnode[index].sumCosts;
}

void sptree(int index, int nodeCnt)
{
    //ansatz: frage nachbar nach root node

    pnode[index].msgCnt++;

    for(int i = 0; i < nodeCnt; i++)
    {
        if (pnode[index].plinks[i].costs > 0)
        {
            if (pnode[index].rootID > getNeighbourRoot(i))
            {
#ifdef DEBUG
                printf("\n(%s %d): The Root of Neighbour(%s) is a better Root: myRoot=%d Root=%d, costs to root=%d",
                        pnode[index].name, pnode[index].nodeID, pnode[i].name, pnode[index].rootID, getNeighbourRoot(i), getNeighbourCoststoRoot(i));
#endif
                pnode[index].rootID = getNeighbourRoot(i);
                pnode[index].nextHop = pnode[i].nodeID;
                pnode[index].sumCosts = getNeighbourCoststoRoot(i) + pnode[index].plinks[i].costs;

#ifdef DEBUG
                pnode[index].changerootCnt++;
#endif
            }
            else if (pnode[index].rootID == getNeighbourRoot(i)) //is there a way with better costs?
            {
                if (pnode[index].sumCosts > getNeighbourCoststoRoot(i) + pnode[index].plinks[i].costs)
                {
#ifdef DEBUG
                    printf("\n(%s %d): Neighbour(%s) have same Root with better costs: Root=%d, my costs to root=%d, neighbour costs to root:%d",
                        pnode[index].name, pnode[index].nodeID, pnode[i].name, pnode[index].rootID, pnode[index].sumCosts, getNeighbourCoststoRoot(i) + pnode[index].plinks[i].costs);
#endif
                    pnode[index].nextHop = pnode[i].nodeID;
                    pnode[index].sumCosts = getNeighbourCoststoRoot(i) + pnode[index].plinks[i].costs;

#ifdef DEBUG
                    pnode[index].changerootCnt++;
#endif
                }
            }
        }
    }
}

bool isTreeFinish(int nodeCnt)
{
    for(int i = 0; i < nodeCnt; i++)
    {
        if(pnode[i].msgCnt < nodeCnt)
        {
            return false;
        }
    }
    return true;
}

int checkGraph(int nodeCnt)
{
	for(int i = 0; i < nodeCnt; i++)
	{
		//that means a Node was used in a Link, but was not deklared
		if(pnode[i].nodeID == NODE_ID_INIT)
		{
			printf("Node %s wurde nicht definiert!\n", pnode[i].name);
			return 0;
		}
	}

    return true;
}

void printTable(int nodeCnt)
{
	printf("\t");
	for(int i = 0; i < nodeCnt; i++)
	{
		printf("| %s\t", pnode[i].name);
	}
	printf("\n");
	
	for(int i = 0; i < nodeCnt; i++)
	{
		printf("%s:%d\t", pnode[i].name, pnode[i].nodeID);
		
		for(int j = 0; j < nodeCnt; j++)
		{
			printf("| %d\t", pnode[i].plinks[j].costs);
		}
		printf("\n");
	}
}

void printTreeResult(int nodeCnt)
{
    printf("\n\n");
    for(int i = 0; i < nodeCnt; i++)
    {
        printf("\nNode %s:%d have a Way to Root(%d) via %d with costs of: %d", pnode[i].name, pnode[i].nodeID, pnode[i].rootID, pnode[i].nextHop, pnode[i].sumCosts);

#ifdef DEBUG
            printf("\t (found in %d of total %d steps)", pnode[i].changerootCnt, pnode[i].msgCnt);
#endif
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
	FILE *fp = stdin;
	int nodeCnt = 0;
   
   	if(argc > 2)
   	{
   		printf("Usage: %s [<file>]\n", argv[0]);
      	return 1;
   	}
   	if(argc == 2)
   	{
      	fp = fopen(argv[1], "r");
      	if(!fp)
      	{
        	printf("Could not open the file!\n");
		 	return 1;
      	}
   	}

	nodeCnt = getGraph(fp);
	
	
	if(checkGraph(nodeCnt) == 0)
	{
		printf("Graph nicht korrekt definiert!\n");
		return 0;
	}
	
	printTable(nodeCnt);
	
   	if(fp!=stdin)
   	{
      	fclose(fp);
   	}

    printf("Nodes: %d\n", nodeCnt);

    srand((int)time(NULL));

    while(!isTreeFinish(nodeCnt))
    {
        sptree(rand() % nodeCnt, nodeCnt);
    }

    printTreeResult(nodeCnt);

   	return 0;
}
