CC = g++
CFLAGS = -Wall -std=c++11 -Os
LDFLAGS =

TARGET = run

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -pg -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -pg -c $(CFLAGS) $^ -o $@

clean:
	rm $(TARGET) $(OBJECTS)
