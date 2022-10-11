ARDUINO_DIR := arduino
ENV_VARS := -DDMGC_SDL_ARDUINO_BUILD
CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -std=c++14 -pedantic -ggdb -g -O0
#CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -std=c++14 -pedantic
#CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -pedantic -ggdb -lSDL2
LDFLAGS := -lSDL2 -lSDL2_ttf -lSDL2_image -lyaml-cpp
CC := g++
INO_FLAG := -x c++

DMGC_IPS_ATTINY85 := dmgc-ips-attiny85
DMGC_IPS_ATTINY85_PLUS := dmgc-ips-attiny85-plus
BREATHING := breathing
BASE_SOURCE := src/arduino_defs.cpp src/EEPROM.cpp src/Adafruit_NeoPixel.cpp arduino/library/dmgc-utils/dmgc-utils.cpp src/configuration.cpp src/main.cpp
COMPILE_LINE := $(SOURCE) $(CFLAGS) $(LDFLAGS)

#.PHONY: build
all: bin $(BREATHING) $(DMGC_IPS_ATTINY85) $(DMGC_IPS_ATTINY85_PLUS)

$(DMGC_IPS_ATTINY85): $(BASE_SOURCE)
	$(CC) $(INO_FLAG) $(ARDUINO_DIR)/ino/$(DMGC_IPS_ATTINY85)/$(DMGC_IPS_ATTINY85).ino $(BASE_SOURCE) $(CFLAGS) $(LDFLAGS) -o bin/$(DMGC_IPS_ATTINY85)

$(DMGC_IPS_ATTINY85_PLUS): $(BASE_SOURCE)
	$(CC) $(INO_FLAG) $(ARDUINO_DIR)/ino/$(DMGC_IPS_ATTINY85_PLUS)/$(DMGC_IPS_ATTINY85_PLUS).ino $(BASE_SOURCE) $(CFLAGS) $(LDFLAGS) -o bin/$(DMGC_IPS_ATTINY85_PLUS)

$(BREATHING): $(BASE_SOURCE)
	$(CC) $(INO_FLAG) $(ARDUINO_DIR)/ino/$(BREATHING)/$(BREATHING).ino $(BASE_SOURCE) $(CFLAGS) $(LDFLAGS) -o bin/$(BREATHING)

bin:
	mkdir -p bin

clean:
	rm -fr bin/$(DMGC_IPS_ATTINY85)
	rm -fr bin/$(DMGC_IPS_ATTINY85_PLUS)
	rm -fr bin/$(BREATHING)
	rmdir bin