BIN=bin
SRC=src

build:
	gcc -g -o $(BIN)/psch $(SRC)/main.c

bt:
	gcc -g -o $(BIN)/test $(SRC)/test.c

clean:
	rm $(BIN)/*
