SOURCE := src/
HEADERS := src/headers/
FLAGS := -g

distracto:
	gcc -o bin/distracto -I$(HEADERS) -I$(SOURCE) main.c $(FLAGS) 
