CC := gcc
CFLAGS := -g -lm -pg -ggdb -O2 -std=c11 -Wall -Wno-missing-braces
TARGET := main 

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC)  -o $@ $^ -lm

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

len:
	find . -name '*.c' | xargs wc -l

clean:
	rm -rf obj/ $(TARGET)

.PHONY: all clean

