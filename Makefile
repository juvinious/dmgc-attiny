CFLAGS := -I ./ -I ./src -Wall -Werror -std=c++14 -pedantic -ggdb -g -O0
#CFLAGS := -I ./ -I ./src -Wall -Werror -std=c++14 -pedantic
#CFLAGS := -I ./ -I ./src -Wall -Werror -pedantic -ggdb -lSDL2
LDFLAGS := -lSDL2 -lSDL2_ttf -lSDL2_image -lyaml-cpp
CC := g++
INO_FLAG := -x c++

DMGC_IPS_ATTINY85 := dmgc-ips-attiny85
BREATHING := breathing
BASE_SOURCE := src/arduino_defs.cpp src/EEPROM.cpp src/Adafruit_NeoPixel.cpp src/configuration.cpp src/main.cpp
COMPILE_LINE := $(SOURCE) $(CFLAGS) $(LDFLAGS)

#.PHONY: build
all: bin $(BREATHING) $(DMGC_IPS_ATTINY85)

$(DMGC_IPS_ATTINY85): $(BASE_SOURCE)
	$(CC) $(INO_FLAG) ino/$(DMGC_IPS_ATTINY85).ino $(BASE_SOURCE) $(CFLAGS) $(LDFLAGS) -o bin/$(DMGC_IPS_ATTINY85)

$(BREATHING): $(BASE_SOURCE)
	$(CC) $(INO_FLAG) ino/$(BREATHING).ino $(BASE_SOURCE) $(CFLAGS) $(LDFLAGS) -o bin/$(BREATHING)

bin:
	mkdir -p bin

clean:
	rm -fr bin/$(DMGC_IPS_ATTINY85)
	rm -fr bin/$(BREATHING)
	rmdir bin