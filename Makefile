ARDUINO_DIR := arduino
OBJ_DIR := obj
BIN_DIR := bin
ENV_VARS := -DDMGC_SDL_ARDUINO_BUILD
CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -std=c++14 -pedantic -ggdb -g -O0 -MMD -MP
#CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -std=c++14 -pedantic -MMD -MP
#CFLAGS := $(ENV_VARS) -I ./ -I ./src -I ./arduino/library/dmgc-utils -Wall -Werror -pedantic -ggdb -MMD -MP

LDFLAGS := -lSDL2 -lSDL2_ttf -lSDL2_image -lyaml-cpp
CC := g++
INO_FLAG := -x c++

BASE_SOURCE := src/arduino_defs.cpp src/EEPROM.cpp src/Adafruit_NeoPixel.cpp arduino/library/dmgc-utils/dmgc-utils.cpp src/configuration.cpp src/main.cpp
OBJ_FILES := $(BASE_SOURCE:%=$(OBJ_DIR)/%.o)

#################### BIN NAMES #########################
DMGC_IPS_ATTINY85 := dmgc-ips-attiny85
DMGC_IPS_ATTINY85_PLUS := dmgc-ips-attiny85-plus
BREATHING := breathing
########################################################

################# BIN SOURCES ##########################
BREATHING_BIN := $(BIN_DIR)/$(BREATHING)
BREATHING_SOURCE := $(ARDUINO_DIR)/ino/$(BREATHING)/pixel-magic.cpp $(ARDUINO_DIR)/ino/$(BREATHING)/$(BREATHING).ino
BREATHING_OBJ_FILES := $(BREATHING_SOURCE:%=$(OBJ_DIR)/%.o)

DMGC_IPS_ATTINY85_PLUS_BIN := $(BIN_DIR)/$(DMGC_IPS_ATTINY85_PLUS)
DMGC_IPS_ATTINY85_PLUS_SOURCE := $(ARDUINO_DIR)/ino/$(DMGC_IPS_ATTINY85_PLUS)/$(DMGC_IPS_ATTINY85_PLUS).ino
DMGC_IPS_ATTINY85_PLUS_OBJ_FILES := $(DMGC_IPS_ATTINY85_PLUS_SOURCE:%=$(OBJ_DIR)/%.o)

DMGC_IPS_ATTINY85_BIN := $(BIN_DIR)/$(DMGC_IPS_ATTINY85)
DMGC_IPS_ATTINY85_SOURCE := $(ARDUINO_DIR)/ino/$(DMGC_IPS_ATTINY85)/$(DMGC_IPS_ATTINY85).ino
DMGC_IPS_ATTINY85_OBJ_FILES := $(DMGC_IPS_ATTINY85_SOURCE:%=$(OBJ_DIR)/%.o)
########################################################

##################### DEPENDENCIES #####################
DEPS := $(OBJ_FILES:.o=.d) $(BREATHING_OBJ_FILES:.o=.d) $(DMGC_IPS_ATTINY85_OBJ_FILES:.o=.d) $(DMGC_IPS_ATTINY85_PLUS_OBJ_FILES:.o=.d)
########################################################

################# ALL TARGETS ##########################
all: $(OBJ_DIR) $(OBJ_FILES) $(INO_OBJ_FILES) $(BIN_DIR) $(BREATHING_BIN) $(DMGC_IPS_ATTINY85_BIN) $(DMGC_IPS_ATTINY85_PLUS_BIN)
.PHONY: all
########################################################

##################### BINS #############################
$(DMGC_IPS_ATTINY85_PLUS_BIN): $(OBJ_FILES) $(DMGC_IPS_ATTINY85_PLUS_OBJ_FILES)
	$(CC) $(DMGC_IPS_ATTINY85_PLUS_OBJ_FILES) $(OBJ_FILES) -o $@ $(LDFLAGS)

$(DMGC_IPS_ATTINY85_BIN): $(OBJ_FILES) $(DMGC_IPS_ATTINY85_OBJ_FILES)
	$(CC) $(DMGC_IPS_ATTINY85_OBJ_FILES) $(OBJ_FILES) -o $@ $(LDFLAGS)
	
$(BREATHING_BIN): $(OBJ_FILES) $(BREATHING_OBJ_FILES)
	$(CC) $(BREATHING_OBJ_FILES) $(OBJ_FILES) -o $@ $(LDFLAGS)
########################################################

$(BIN_FILES): $(OBJ_FILES) $(INO_OBJ_FILES)
	$(CC) $(OBJ_DIR)/arduino/ino/$(@F)/$(@F).ino.o $(OBJ_FILES) -o $@ $(LDFLAGS)

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

-include $(DEPS)