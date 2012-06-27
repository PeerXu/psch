BIN=bin
SRC=src

build:
	gcc -g -c -o $(BIN)/object.o $(SRC)/object.c
	gcc -g -c -o $(BIN)/main.o $(SRC)/main.c
	gcc -g -o $(BIN)/psch $(BIN)/main.o $(BIN)/object.o

bt:
	gcc -g -o $(BIN)/test $(SRC)/test.c

clean:
	rm $(BIN)/*