CC = g++
OBJS = Main.o Options.o Energy.o Align.o Util.o Sequence.o 
DEBUG = -pg
LIBS = -lz 
CFLAGS = -std=c++11 -Wall -c -O4 
LFLAGS = -Wall 
EXE = mirinho 

extractor: $(OBJS)
	$(CC) $(LFLAGS) $(LIBS) $(OBJS) -o $(EXE)

Main.o: Main.cpp Options.hpp Energy.hpp Definitions.hpp Align.hpp Sequence.hpp Util.hpp kseq.h
	$(CC) $(CFLAGS)  Main.cpp

Options.o: Options.cpp Options.hpp
	$(CC) $(CFLAGS)  Options.cpp

Energy.o: Energy.cpp Energy.hpp Options.hpp Definitions.hpp Util.hpp
	$(CC) $(CFLAGS)  Energy.cpp

Align.o: Align.cpp Align.hpp Definitions.hpp Energy.hpp Util.hpp
	$(CC) $(CFLAGS)  Align.cpp

Util.o: Util.cpp Util.hpp Options.hpp Definitions.hpp
	$(CC) $(CFLAGS)  Util.cpp

Sequence.o: Sequence.cpp Sequence.hpp 
	$(CC) $(CFLAGS)  Sequence.cpp

.PHONY: clean

clean:
	rm -f $(EXE) *.BAK *.gch *.o gmon.out
