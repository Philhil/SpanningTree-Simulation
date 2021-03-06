#include "sim.h"

node *pnode = NULL;
char graphname[MAX_IDENT];

//Read, validate and build pnode data Array from input graph.
//param: filepointer
//return: number of nodes
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
                printf("\nline read:%s(Node extracted: %s/id of node: %d)\n", line, nodeA, number);
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
						//Node is already there, but no ID defined
						if(pnode[indexA].nodeID == NODE_ID_INIT)
						{
							pnode[indexA].nodeID = number;
                            pnode[indexA].rootID = number;
                            pnode[indexA].nextHop = number;
							#ifdef DEBUG
							printf("id %d appended to node %s\n", number, pnode[indexA].name);
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
						printf("node %s id %d appended\n", nodeA, number);
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
                printf("\nline read:%s(Nodes extracted: %s,%s/Costs for link: %d)\n", line, nodeA, nodeB, number);
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
						printf("node %s appended (id: %d)\n", nodeA, NODE_ID_INIT);
						#endif
					}
					if(getIndex(nodeB, nodeCnt) == -1)
					{
                    	nodeCnt = appendNode(nodeB, nodeCnt);
						if(nodeCnt == 0) return 0;
						#ifdef DEBUG
						printf("node %s appended (id: %d)\n", nodeB, NODE_ID_INIT);
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
				/*
					Not really elegant, but it works
					didn't know how to ignore the first string "Graph" but it isn't needed so you can scan and then override it
				*/
				sscanf(line, "%s %s", graphName, graphName);
				strcpy(graphname, graphName);
				#ifdef DEBUG
				printf("(name): %s\n", graphname);
				#endif
				break;
			//end line
			case typeGraph_end:
				return nodeCnt;
		}
    }

    return 0;
}

//check each line of input graph to decide the line type
//input: line
//return: linetype from enum
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

//Validate node name. Starting with Alpha char and not longer as MAX_IDENT
//input: nodename
//return: 0|1
int isValid(char *string)
{
	int count = 0;

	if(isalpha(*string) == 0) return 0;
	count++;
	string++;

	while(count <= MAX_IDENT)
	{
		//if name has ended
		if(*string == '\0') return 1;
		//if name contains illegal char
		if(isalpha(*string) == 0 && isdigit(*string) == 0) return 0;
		count++;
		string++;
	}
	return 0; //this should be never reached
}

//search Index of a nodename
//input: nodename, totalNodes in pNode array
//return: return the index or -1
int getIndex(char *name, int nodeCnt)
{
	for(int i = 0; i < nodeCnt; i++)
	{
		if(strcmp(pnode[i].name, name) == 0)
			return i;
	}
	
    return -1;
}

//search the nodename of a nodename
//input: noedid, namepointer, nodecount
//return: 0|1
int getNameFromID(int id, char *name, int nodeCnt)
{
    for(int i = 0; i < nodeCnt; i++)
    {
        if(pnode[i].nodeID == id)
        {
            strcpy(name, pnode[i].name);
            return 1;
        }
    }
    return 0;
}

//search the nodename at index
//input: name pointer, index
void getName(char *name, int index)
{
	strcpy(name, pnode[index].name);
}

//search nodeId at index
//input: index
//return: nodeId
int getID(int index)
{
	return pnode[index].nodeID;
}

//add a node to the global array pnode and manage Array size.
//input: nodeName (should already be valid), nodeCount
//return: nodeCount
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

//set the correct size of link array in each node and initialize the link costs with 0
//input: nodeCount
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

//get the rootid of Neighbour node
//input: index
//return: neighbour rootID
int getNeighbourRoot(int index)
{
    return pnode[index].rootID;
}

//get the rootcosts of Neighbour node
//input: index
//return: neighbour rootcosts
int getNeighbourCoststoRoot(int index)
{
    return pnode[index].sumCosts;
}

//function realize spanningtree simulation of a single node. The node ask neighbours and if necessary set its own properties.
//input: index of node, nodeCount
void sptree(int index, int nodeCnt)
{
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

//check if cancel condition of spanningtree algorithm is reached
//input: nodecount
//return: 0 | 1
int isTreeFinish(int nodeCnt)
{
    for(int i = 0; i < nodeCnt; i++)
    {
        if(pnode[i].msgCnt < nodeCnt)
        {
            return 0;
        }
    }
    return 1;
}

//validate if graph have start conditions
//input: nodecount
//return 0|1
int checkGraph(int nodeCnt)
{
	int rootCnt = 0;
	
	//check if all nodes id's are greater than 0 
	for(int i = 0; i < nodeCnt; i++)
	{
		//that means a Node was used in a Link, but was not deklared
		if(pnode[i].nodeID <= 0)
		{
			printf("Node %s wurde nicht oder falsch definiert!\n", pnode[i].name);
			return 0;
		}
		if(pnode[i].nodeID == 1)
		{
			rootCnt ++;
			if(rootCnt > 1)
			{
				printf("Es wurden mehr als eine Root definiert!\n");
				return 0;
			}
		}
		for(i = 0; i < nodeCnt; i++)
		{
			for(int j = 0; j < nodeCnt; j++)
			{
				if(i == j)
				{
					if(pnode[i].plinks[j].costs != 0)
					{
						printf("Knoten %s ist mit sich selbst verbunden!\n", pnode[i].name);
						return 0;
					}
				}
			}
		}
		if(checkIfConnected(nodeCnt) == 0)
		{
			return 0;
		}
	}

    return 1;
}

//checks if the graph is connected
//input: nodecount
//retunr: 0|1
int checkIfConnected(int nodeCnt)
{
	int *list = NULL;
	int *tmp = NULL;
	
	tmp = (int*) calloc(nodeCnt, sizeof(int));
	if(tmp == NULL)
	{
		//error in calloc
		printf("Fehler beim calloc der Liste!\n");
		free(pnode);
		return 0;
	}
	list = tmp;
	
	checkPaths(pnode[0].name, nodeCnt, list);
	
	for(int i = 0; i < nodeCnt; i++)
	{
		if(list[i] == 0)
		{
			printf("Der Graph ist nicht verbunden!\n");
			return 0;
		}
	}
	
	return 1;
}

//checks all Paths from a given node
int checkPaths(char *name, int nodeCnt, int *list)
{
	int index;
	char newname[MAX_IDENT];
	int id;
	
	index = getIndex(name, nodeCnt);
	
	id = getID(index);
	if(fillList(list, id, nodeCnt) > -1)
	{
		#ifdef DEBUG
		printf("added id %d to list\n", id);
		#endif
	}
	else
	{
		return 0;
	}
	
	for(int i = 0; i < nodeCnt; i++)
	{		
		if(pnode[index].plinks[i].costs > 0)
		{
			getName(newname, i);
			#ifdef DEBUG
			printf("check connections for %s\n", newname);
			#endif
			checkPaths(newname, nodeCnt, list);
		}
	}
	
	return 0;
}

//add a given id to a given list
//input: listpointet, id, nodecount
//return: index or -1 on error (already in list or list is full)
int fillList(int *list, int id, int nodeCnt)
{
	for(int i = 0; i < nodeCnt; i++)
	{
		if(list[i] == id)
		{
			return -1;
		}
		else if(list[i] == 0)
		{
			list[i] = id;
			return i;
		}
	}
	
	return -1;
}

//print the current graph
//input: nodecount
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
	
	printf("Nodes: %d\n", nodeCnt);
}

//print current Spanningtree result
//input: nodecount
void printTreeResult(int nodeCnt)
{
	char nodeName[MAX_IDENT+1];
	
    printf("\n\nSpanning-Tree of %s{", graphname);
	if(getNameFromID(pnode[0].rootID, nodeName, nodeCnt) == 0)
	{
		printf("Node nicht gefunden!\n");
		return;
	}
	printf("\n\tRoot: %s;", nodeName);
    for(int i = 0; i < nodeCnt; i++)
    {
		if(pnode[i].nodeID != pnode[i].rootID)
        {
			if(getNameFromID(pnode[i].nextHop, nodeName, nodeCnt) == 0)
			{
				printf("Node nicht gefunden!\n");
				return;
			}
			printf("\n\t%s - %s;", pnode[i].name, nodeName);
		}
    }
    printf("\n}\n");
	
	#ifdef DEBUG
    for(int i = 0; i < nodeCnt; i++)
    {
        printf("\nNode %s:%d has a Way to Root(%d) via %d with costs of: %d", pnode[i].name, pnode[i].nodeID, pnode[i].rootID, pnode[i].nextHop, pnode[i].sumCosts);
        printf("\t (found in %d of total %d steps)", pnode[i].changerootCnt, pnode[i].msgCnt);
    }
	printf("\n");
	#endif
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

    //read the given Graph
	nodeCnt = getGraph(fp);
	
	//validate the graph
	if(checkGraph(nodeCnt) == 0)
	{
		printf("Graph nicht korrekt definiert!\n");
		return 0;
	}
	
	#ifdef DEBUG
	printTable(nodeCnt);
	#endif
	
   	if(fp!=stdin)
   	{
      	fclose(fp);
   	}

    srand((int)time(NULL));

    //run the spanningtree algorithm on a random node as long as the finish condition is not reached
    while(isTreeFinish(nodeCnt) == 0)
    {
        sptree(rand() % nodeCnt, nodeCnt);
    }

    printTreeResult(nodeCnt);

   	return 0;
}
