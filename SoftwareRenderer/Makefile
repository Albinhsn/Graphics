CC := gcc
CFLAGS := -O2 -g -std=c11 -Wall -Wno-missing-braces  -I./src/common -I./src/game
TARGET := main 
LDFLAGS := -ldl -lSDL2 -lm -lz -lGL

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c,obj/%.o,$(SRCS))

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC)  -o $@ $^ $(LDFLAGS)

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf obj/ $(TARGET)

.PHONY: all clean

