PROG= bin/server
LIB= $(wildcard header/*.h)
SRC= $(wildcard src/*.c)
OBJ= $(wildcard obj/*.o)
build: $(OBJ) 
	gcc -o $(PROG) $(OBJ) -Iheader
compile: $(SRC) $(LIB)
	gcc -c $(SRC) -Iheader
	mv *.o obj
clean: 
	rm $(PROG) $(OBJ) 2> /dev/null 
