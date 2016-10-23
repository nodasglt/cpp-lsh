CC = g++
CFLAGS = -Wall -std=c++14 -Os -pedantic-errors
LDFLAGS = #-pthread
INCLUDE = -I ./src/
PROFILE = #-pg --coverage

TARGET = run

OBJECTS = $(patsubst %.cpp, %.o, $(shell find ./src/ -type f -name '*.cpp'))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(PROFILE) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(INCLUDE) -c $(PROFILE) $(CFLAGS) $^ -o $@

clean:
	rm $(TARGET) $(OBJECTS)
	rm -r ./Data
	rm massif.out.* gmon.out

profile:
	mkdir ./Data
	find ./src/ -name "*.gcno" -exec mv {} ./Data/ \;
	find ./src/ -name "*.gcda" -exec mv {} ./Data/ \;
	lcov -t "result" -o ./Data/ex_test.info -c -d ./Data
	genhtml -o ./Data/res ./Data/ex_test.info
