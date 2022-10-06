#include <stdio.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "arduino_defs.h"
#include "configuration.h"


int main(int argc,char* argv[])
{
	Configuration * config = Configuration::Get();

	if (!config->setup()) {
		return 0;
	}

	// Setup arduino call
	setup();

	config->toggleSetupCompleted();
	
	bool running = true;
	while (running){
		
		config->handleKeys(running);
		
		config->screenClear();

		// Arduino loop
		loop();

		config->setText("Current color - red: " + std::to_string(config->getPixelColor().r) + " | green: " + std::to_string(config->getPixelColor().g) + " | blue " + std::to_string(config->getPixelColor().b));

		config->renderText(0, config->getHeight() - 40, config->getWidth(), 30);

		config->screenRender();
	}

	Configuration::Destroy();

	return 0;
}