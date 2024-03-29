BIN=bin
SRC=src

build:
	gcc -g -c -o $(BIN)/object.o $(SRC)/object.c
	gcc -g -c -o $(BIN)/main.o $(SRC)/main.c
	gcc -g -o $(BIN)/psch $(BIN)/main.o $(BIN)/object.o

bt:
	gcc -g -c -o $(BIN)/skiplist.o $(SRC)/skiplist.c
	gcc -g -c -o $(BIN)/test.o $(SRC)/test.c
	gcc -g -o $(BIN)/test $(BIN)/skiplist.o $(BIN)/test.o

clean:
	rm $(BIN)/*
