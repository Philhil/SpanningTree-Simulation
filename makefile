PARAMS = -Wall
OBJ = *.o spantree

all: spantree

debug: PARAMS += -D DEBUG
debug: spantree

clean :
	rm -rf $(OBJ)

spantree: sim.o sim_utils.o list.o
	gcc -o spantree sim.o sim_utils.o list.o $(PARAMS)
	rm *.o
	sleep 3
	clear
	./spantree graph.txt

%.o : %.c
	gcc -c $< $(PARAMS)