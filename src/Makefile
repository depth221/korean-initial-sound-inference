CC = g++
CXXFLAGS = -Wall -O2
LDLIBS = -lpthread -ldl
TARGET = main

$(TARGET): sqlite3/sqlite3.o sqlite3/shell.o main.o parseSounds.o
	$(CC) $(CXXFLAGS) $(LDLIBS) $^ -o $@

sqlite3/sqlite3.o: sqlite3/sqlite3.c sqlite3/sqlite3.h
	gcc $(CXXFLAGS) $(LDLIBS) -c sqlite3/sqlite3.c -o sqlite3/sqlite3.o

sqlite3/shell.o: sqlite3/shell.c
	gcc $(CXXFLAGS) $(LDLIBS) -c sqlite3/shell.c -o sqlite3/shell.o

parseSounds.o: parseSounds.cpp parseSounds.hpp
	$(CC) $(CXXFLAGS) $(LDLIBS) -c $<

main.o: main.cpp
	$(CC) $(CXXFLAGS) $(LDLIBS) -c $<

all: $(TARGET)

clean:
	rm -f *.o
	rm -f sqlite3/*.o
	rm -f $(TARGET)
