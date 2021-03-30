OPTIONS_FLAGS = -D_DEBUG
OBJ_DIR = Obj/

physics.o: Src/physics.c
	gcc -Wall -c Src/physics.c $(OPTIONS_FLAGS) -I Includes/ -o $(OBJ_DIR)/physics.o

all: main.c physics.o
	gcc -Wall physics.o main.c -o main -lm $(OPTIONS_FLAGS) -lpthread -I Includes/
