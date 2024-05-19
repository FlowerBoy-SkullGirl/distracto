SOURCE := src/
HEADERS := src/headers/
FLAGS := -lncurses -g

list_test:
	gcc -o test/list_test -I$(HEADERS) -I$(SOURCE) test/list_test.c src/distracto_errors.c $(FLAGS)

distracto:
	gcc -o bin/distracto -I$(HEADERS) -I$(SOURCE) $(SOURCE)*.c $(FLAGS) 
