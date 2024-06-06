CC := g++
CFLAGS := -g -lm -lSDL2 -lGL -O2 -std=c++11 -Wall -Werror
TARGET := main 

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp,obj/%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC)  -o $@ $^ $(CFLAGS) 

obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

len:
	find . -name '*.cpp' | xargs wc -l

clean:
	rm -rf obj/ $(TARGET)

.PHONY: all clean

