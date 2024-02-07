CC := gcc
CFLAGS := -g -pg -ggdb -O3 -std=c11 -Wall -Wno-missing-braces
TARGET := main 

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC)  -pg -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf obj/ $(TARGET)

.PHONY: all clean

