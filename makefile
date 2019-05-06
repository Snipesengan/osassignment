CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
OBJ = LinkedList.o Queue.o scheduler.o  
EXEC = scheduler

$(EXEC): $(OBJ) $(S_OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(S_OBJ) -o $(EXEC) -lpthread 

scheduler.o: scheduler.c scheduler.h Queue.h
	$(CC) $(CFLAGS) -c scheduler.c

Queue.o: Queue.c Queue.h LinkedList.h
	$(CC) $(CFLAGS) -c Queue.c

LinkedList.o: LinkedList.c LinkedList.h
	$(CC) $(CFLAGS) -c LinkedList.c

clean:
	rm -f $(OBJ) $(EXEC)
