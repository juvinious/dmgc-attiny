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
    int brightness; 
    SDL_Color ledColor;

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

    void renderLeds();

    void delay(int override = -1);

    void setTotalLeds (int leds);
    int getTotalLeds() const;

    void setBrightness(int brightness) {
        this->brightness = brightness;
    }

    int getBrightness() const {
        return this->brightness;
    }

    void setPixelColor(int r, int g, int b);

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	TTF_Font * font;
    SDL_Surface * fontSurface;
    SDL_Texture * fontTexture;
};

#endif