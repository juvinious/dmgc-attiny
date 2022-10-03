#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <yaml-cpp/yaml.h>

class Configuration {
private:
	YAML::Node yaml;
	std::string title;
	int windowX;
	int windowY;
	std::string openFont;
public:
	Configuration();
	~Configuration();

	bool setup();

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	TTF_Font * font;
};

#endif