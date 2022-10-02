NAME := main
OUT := -o main
CFLAGS := -I ./ -I ./src -Wall -Werror -std=c++14 -pedantic -ggdb
#CFLAGS := -I ./ -I ./src -Wall -Werror -pedantic -ggdb -lSDL2
LDFLAGS := -lSDL2
CC := g++

ino := -x c++ src/dmgc-ips-attiny85.ino
source := src/globals.cpp src/EEPROM.cpp src/Adafruit_NeoPixel.cpp
main := src/main.cpp

.PHONY: build

build: $(source)
	$(CC) $(source) $(ino) $(main) $(CFLAGS) $(OUT) $(LDFLAGS)

run:
	./$(NAME)

clean:
	rm -fr $(NAME)
