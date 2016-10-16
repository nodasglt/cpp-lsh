CC = g++
CFLAGS = -Wall -std=c++11 -Os -pedantic-errors
LDFLAGS =
INCLUDE = -I ./src/

TARGET = run

OBJECTS = $(patsubst %.cpp, %.o, $(shell find ./src/ -type f -name '*.cpp'))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(INCLUDE) -c $(CFLAGS) $^ -o $@

clean:
	rm $(TARGET) $(OBJECTS) massif.out.* gmon.out
