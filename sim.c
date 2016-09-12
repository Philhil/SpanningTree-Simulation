#include "sim.h"
#include "sim_utils.h"

node *pnode = NULL;

int main(int argc, char *argv[])
{
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

	getGraph(fp);

   	if(fp!=stdin)
   	{
      	fclose(fp);
   	}
   
   	return 0;
}