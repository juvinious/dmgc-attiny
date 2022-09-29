NAME := main
OUT := -o main
CFLAGS := -I ./ -Wall -Werror -std=c++14 -pedantic -ggdb
#CFLAGS := -I ./ -Wall -Werror -pedantic -ggdb -lSDL2
CC := g++

source := Adafruit_NeoPixel.cpp EEPROM.cpp dmgc-ips-attiny85.ino main.cpp

.PHONY: build

build: $(source)
	echo $(source)
	$(CC) $(source) $(CFLAGS) $(OUT)

run:
	./$(NAME)

clean:
	rm -fr $(NAME)
