#include <stdio.h>
#include <SDL2/SDL.h>
#include "dmgc-ips-attiny85.h"

#include "yaml-cpp/yaml.h"

using namespace YAML;
const std::string CONFIGURATION_FILE = "config.yaml";

class Configuration {
private:
	Node yaml;
	std::string title;
	int windowX;
	int windowY;
public:
	Configuration():
	yaml(LoadFile(CONFIGURATION_FILE)),
	title("main"),
	window(NULL),
	renderer(NULL){
		title = yaml["title"].as<std::string>();
		windowX = yaml["window-x"].as<int>();
		windowY = yaml["window-y"].as<int>();
	}
	~Configuration() {
		if (renderer != NULL) {
			SDL_DestroyRenderer(renderer);
		}
		if (window != NULL) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
	}

	bool setup(){
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("error initializing sdl. %s\n", SDL_GetError());
			return false;
		}

		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, 0);
		if (!window)
		{
			printf("Error creating window: %s\n", SDL_GetError());
			//SDL_Quit();
			return false;
		}
		/* Create a renderer */
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer)
		{
			printf("Error creating renderer: %s\n", SDL_GetError());
			//SDL_DestroyWindow(window);
			//SDL_Quit();
			return false;
		}

		return true;
	}

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
};

struct _CONFIG {
	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
} CONFIG;

int main(int argc,char* argv[]){

	/*if (!SDL2_Setup()){
		return 0;
	}*/

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

	//SDL2_Cleanup();

	return 0;
}