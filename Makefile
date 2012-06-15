BIN=bin
SRC=src

build:
	gcc -o $(BIN)/hello $(SRC)/main.c

clean:
	rm $(BIN)/*
