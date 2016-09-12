spantree: sim.o sim_utils.o
	gcc -o spantree sim.o sim_utils.o
	rm *.o
	sleep 3
	clear
	./spantree graph.txt

sim.o: sim.c
	gcc -c sim.c

sim_utils.o : sim_utils.c
	gcc -c sim_utils.c
