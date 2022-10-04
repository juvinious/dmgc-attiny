#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <yaml-cpp/yaml.h>

class Led
{
    public:
    Led();
    ~Led();

    SDL_Rect coords;
    SDL_Color color;
};

class Configuration 
{
private:
	YAML::Node yaml;
	std::string title;
	int windowX;
	int windowY;
	std::string openFont;
    int ticks;
    int frames;

    int totalLeds;
    std::vector<Led *> leds;

    static Configuration * _config;
	Configuration();

public:
	~Configuration();

    static Configuration * Get();
    static void Destroy();

	bool setup();

    void createText(std::string message);
    void renderText();

    void screenClear();
    void screenRender();

    void delay(int override = -1);

    void setTotalLeds (int leds);

    int getTotalLeds() const;

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	TTF_Font * font;
    SDL_Surface * fontSurface;
    SDL_Texture * fontTexture;
};

#endif