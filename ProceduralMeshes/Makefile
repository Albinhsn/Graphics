CC := gcc
CFLAGS := -O0 -g -std=c11 -Wall 
TARGET := main 
LDFLAGS := -lGL -ldl -lSDL2 -lm -lz

LIBRARIES := -I./include/

SRCS := $(wildcard ./src/*.c)
LIBS := $(wildcard ./lib/*.c) 
OBJS := $(patsubst ./src/%.c,./obj/%.o,$(SRCS)) $(patsubst ./lib/%.c,./obj/%.o,$(LIBS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBRARIES)
./obj/%.o: ./src/%.c
	$(CC) $(CFLAGS) $(LIBRARIES)  -c $<  && mv *.o ./obj/
./obj/%.o: ./lib/%.c
	$(CC) $(CFLAGS) $(LIBRARIES)  -c $<  && mv *.o ./obj/
clean:
	rm -rf $(TARGET) ./obj/*.o

len:
	find ./src -name '*.c' | xargs wc -l


.PHONY: all clean

