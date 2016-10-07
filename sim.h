#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "list.h"

#define MAX_IDENT		10		//length of identifier for node	
#define MAX_ITEMS		256		//max number of rows in the input data 
#define MAX_COSTS		256 	//max costs for one link
#define MAX_NODE_ID		256		//max costs for one node
#define MAX_LINE		256		//max chars in one line

//structure for link
/*typedef struct{
	int costs;					//costs for link between two nodes
								//if 0 -> no link exists
	
	int rootID;					//assumed root
	int sumCosts;				//total costs to root
}link;*/

//Datastructure for one node in the matrix of all nodes
typedef struct{
	char name[MAX_IDENT+1];		//name of the node
	int nodeID;					//node id >0
	list *plinks;				//list of potential reachable nodes
	int nextHop;				//calculated link to the next node in direction of root
	int msgCnt;					//counts how many times the node was used
} node;


void free_node(void *data);

bool iterate_node_print(void *data);
bool compare_node_name(void *comparisonobj, void *data);