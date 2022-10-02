#include <stdio.h>
#include <SDL2/SDL.h>
#include "dmgc-ips-attiny85.h"

struct _CONFIG {
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
} CONFIG;


int SDL2_Setup() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("error initializing sdl. %s\n", SDL_GetError());
		return 0;
	}

	CONFIG.window = SDL_CreateWindow("DMGC Attiny simulator.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	if (!CONFIG.window)
	{
		printf("Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}
	/* Create a renderer */
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	CONFIG.renderer = SDL_CreateRenderer(CONFIG.window, -1, render_flags);
	if (!CONFIG.renderer)
	{
		printf("Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(CONFIG.window);
		SDL_Quit();
		return 0;
	}
	return 1;
}

void SDL2_Cleanup() {
	SDL_DestroyRenderer(CONFIG.renderer);
	SDL_DestroyWindow(CONFIG.window);
	SDL_Quit();
}

int main(int argc,char* argv[]){

	if (!SDL2_Setup()){
		return 0;
	}

	int running = 1;
	while (running){
		while(SDL_PollEvent(&CONFIG.event))
		{
			switch(CONFIG.event.type)
			{
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					break;
				default:
					break;
			}
		}
		SDL_SetRenderDrawColor(CONFIG.renderer, 0, 0, 0, 255);
		SDL_RenderClear(CONFIG.renderer);

		SDL_RenderPresent(CONFIG.renderer);
		SDL_Delay(1000/30);
	}

	SDL2_Cleanup();

	return 0;
}