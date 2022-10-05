#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "dmgc-ips-attiny85.h"
#include "configuration.h"


int main(int argc,char* argv[])
{
	Configuration * config = Configuration::Get();

	if (!config->setup()) {
		return 0;
	}

	// Setup arduino call
	setup();

	config->createText("Testing....");
	
	bool running = true;
	while (running){
		while(SDL_PollEvent(&config->event))
		{
			switch(config->event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					break;
				default:
					break;
			}
		}
		
		config->screenClear();

		config->renderText();

		// config->renderBackground();

		loop();

		config->screenRender();

		//config->delay();
	}

	Configuration::Destroy();

	return 0;
}