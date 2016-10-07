#include "sim.h"
#include "sim_utils.h"

int main(int argc, char *argv[])
{
    list nodelist;
    list_new(&nodelist, sizeof(node), free_node);

	FILE *fp = stdin;
   
   	if(argc > 2)
   	{
   		fprintf(stderr, "Usage: %s [<file>]\n", argv[0]);
      	return 1;
   	}
   	if(argc == 2)
   	{
      	fp = fopen(argv[1], "r");
      	if(!fp)
      	{
        	printf("Could not open the file!");
		 	return 1;
      	}
   	}

	getGraph(fp, &nodelist);

   	if(fp!=stdin)
   	{
      	fclose(fp);
   	}

    list_for_each(&nodelist, iterate_node_print);

   	return 0;
}

void free_node(void *data)
{
    free(*(node **)data);
}

bool iterate_node_print(void *data)
{
    node node1 = (* (node*) data);
#ifdef DEBUG
    printf("In nodelist: %s\n", node1.name);
#endif
    return true;
}

bool compare_node_name(void *nodename, void *data)
{
    node node1 = (* (node*) data);
#ifdef DEBUG
    printf("compare nodelist: %s and %s\n", nodename, node1.name);
#endif
    return  strcmp(nodename, node1.name) == 0;
}