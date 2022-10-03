#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "dmgc-ips-attiny85.h"
#include "configuration.h"


int main(int argc,char* argv[]){

	Configuration config;
	
	if (!config.setup()) {
		return 0;
	}
	
	bool running = true;
	while (running){
		while(SDL_PollEvent(&config.event))
		{
			switch(config.event.type)
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
		SDL_SetRenderDrawColor(config.renderer, 0, 0, 0, 255);
		SDL_RenderClear(config.renderer);

		SDL_RenderPresent(config.renderer);
		SDL_Delay(1000/30);
	}

	return 0;
}