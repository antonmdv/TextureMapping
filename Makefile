COMPILER = g++
COMPILER_FLAGS = -c -O0 -w -I/usr/local/include
LINKER = g++
LINKER_FLAGS = -framework GLUT -framework OpenGl

all : teapot

teapot : main.o vectorfunctions.o readppm.o readOBJ.o
	$(LINKER) $(LINKER_FLAGS) main.o vectorfunctions.o readppm.o readOBJ.o -o teapot

main.o : main.cpp vectorfunctions.h readppm.h readOBJ.h
	$(COMPILER) $(COMPILER_FLAGS) main.cpp

vectorfunctions.o : vectorfunctions.h vectorfunctions.cpp
	$(COMPILER) $(COMPILER_FLAGS) vectorfunctions.cpp

readppm.o : readppm.h readppm.cpp
	$(COMPILER) $(COMPILER_FLAGS) readppm.cpp

readOBJ.o : readOBJ.h readOBJ.cpp
	$(COMPILER) $(COMPILER_FLAGS) readOBJ.cpp

.PHONY : clean
clean :
	rm -f *.o teapot
