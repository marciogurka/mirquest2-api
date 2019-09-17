CC = g++
CFLAGS = -g -Wall
CXXFLAGS += -std=c++11
EXEC = miRBoost
SRC = miRBoost.cpp
OBJ = $(SRC:.cpp=.o)

all : $(EXEC)

%.o : %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $*.cpp

miRBoost : miRBoost.o
	$(CC) $(CFLAGS) $(CXXFLAGS) -o $@ $^ 

clean :
	rm -f *~ $(OBJ) $(EXEC)
