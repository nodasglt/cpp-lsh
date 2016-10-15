CC = g++
CFLAGS = -Wall -std=c++11 -Os
LDFLAGS =

TARGET = run

OBJECTS = $(patsubst %.cpp, %.o, $(shell find ./src/ -type f -name '*.cpp'))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm $(TARGET) $(OBJECTS)
