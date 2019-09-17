CC = g++
CFLAGS = -g -Wall
EXEC = miRBoost
SRC = miRBoost.cpp
OBJ = $(SRC:.cpp=.o)

all : $(EXEC)

%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp

miRBoost : miRBoost.o
	$(CC) $(CFLAGS) -o $@ $^ 

clean :
	rm -f *~ $(OBJ) $(EXEC)
