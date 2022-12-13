SHELL := /bin/bash
PROG= bin/server
LIB= $(wildcard header/*.h)
SRC= $(wildcard src/*.c)
OBJ= $(wildcard obj/*.o)
build: $(OBJ) 
	gcc -o $(PROG) $(OBJ) -lwiringPi
compile: $(SRC) $(LIB)
	echo "Compiling for `uname -n`"
	# replace "wiringPi.h" with <wiringPi.h> during compile if on Raspberry Pi
	if [ `uname -n` = "pi" ]; then sed -i "s/\"wiringPi\.h\"/<wiringPi.h>/g" header/driver.h; fi
	gcc -c $(SRC) $(LIB) -Iheader
	mv *.o obj
clean: 
	rm $(PROG) $(OBJ) 2> /dev/null 
