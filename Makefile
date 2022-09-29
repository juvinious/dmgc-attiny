NAME := main
OUT := -o main
CFLAGS := -I ./ -I ./src -Wall -Werror -std=c++14 -pedantic -ggdb
#CFLAGS := -I ./ -I ./src -Wall -Werror -pedantic -ggdb -lSDL2
CC := g++


source := src/Adafruit_NeoPixel.cpp src/EEPROM.cpp src/dmgc-ips-attiny85.ino src/main.cpp

.PHONY: build

build: $(source)
	echo $(source)
	$(CC) $(source) $(CFLAGS) $(OUT)

run:
	./$(NAME)

clean:
	rm -fr $(NAME)
