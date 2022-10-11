ARDUINO_DIR := arduino
OBJ_DIR := obj
BIN_DIR := bin
ENV_VARS := -DDMGC_SDL_ARDUINO_BUILD
CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -std=c++14 -pedantic -ggdb -g -O0
#CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -std=c++14 -pedantic
#CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -pedantic -ggdb

LDFLAGS := -lSDL2 -lSDL2_ttf -lSDL2_image -lyaml-cpp
CC := g++
INO_FLAG := -x c++

DMGC_IPS_ATTINY85 := dmgc-ips-attiny85
DMGC_IPS_ATTINY85_PLUS := dmgc-ips-attiny85-plus
BREATHING := breathing
BIN_FILES := $(BIN_DIR)/$(DMGC_IPS_ATTINY85) $(BIN_DIR)/$(DMGC_IPS_ATTINY85_PLUS) $(BIN_DIR)/$(BREATHING)
INO_FILES := $(ARDUINO_DIR)/ino/$(BREATHING)/$(BREATHING).ino $(ARDUINO_DIR)/ino/$(DMGC_IPS_ATTINY85)/$(DMGC_IPS_ATTINY85).ino $(ARDUINO_DIR)/ino/$(DMGC_IPS_ATTINY85_PLUS)/$(DMGC_IPS_ATTINY85_PLUS).ino 

BASE_SOURCE := src/arduino_defs.cpp src/EEPROM.cpp src/Adafruit_NeoPixel.cpp arduino/library/dmgc-utils/dmgc-utils.cpp src/configuration.cpp src/main.cpp
OBJ_FILES := $(BASE_SOURCE:%=$(OBJ_DIR)/%.o)
INO_OBJ_FILES := $(INO_FILES:%=$(OBJ_DIR)/%.o)

all: $(OBJ_DIR) $(OBJ_FILES) $(INO_OBJ_FILES) $(BIN_DIR) $(BIN_FILES)

.PHONY: all

$(BIN_FILES): $(OBJ_FILES)
	$(CC) $(OBJ_DIR)/arduino/ino/$(subst $(BIN_DIR)/,,$@)/$(subst $(BIN_DIR)/,,$@).ino.o $(OBJ_FILES) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.ino.o: %.ino
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $(INO_FLAG) $< -o $@

$(OBJ_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -fr $(BIN_DIR)/$(DMGC_IPS_ATTINY85)
	rm -fr $(BIN_DIR)/$(DMGC_IPS_ATTINY85_PLUS)
	rm -fr $(BIN_DIR)/$(BREATHING)
	-rmdir bin
	rm -fr $(OBJ_DIR)