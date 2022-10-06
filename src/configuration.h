#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#include <vector>
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <yaml-cpp/yaml.h>

/*
class Key
{
    public:
    int originalValue;
    int remappedValue;
};
*/
enum KEYTYPE
{
    NAVIGATION = SDLK_a,
    NAVIGATION_UP = SDLK_a,
    NAVIGATION_DOWN = SDLK_a,
    UP = SDLK_UP,
    DOWN = SDLK_DOWN,
    LEFT = SDLK_LEFT,
    RIGHT = SDLK_RIGHT,
    SELECT = SDLK_TAB,
    START = SDLK_RETURN,
    A = SDLK_a,
    B = SDLK_b
};

class KeyValue {
public:
    KeyValue(enum KEYTYPE type);
    KeyValue(enum KEYTYPE type, int value);
    ~KeyValue();

    void updateValue(int value){
        this->value = value;
    }

    void press() {
        this->down = true;
    }
    
    void release() {
        this->down = false;
    }

    bool isDown() const {
        return this->down;
    }

private:
    enum KEYTYPE type;
    int value;
    bool down;
};

class KeyHandler 
{
public:
    KeyHandler();
    ~KeyHandler();

    void setKey(enum KEYTYPE type, int value);

    KeyValue * getKey(enum KEYTYPE type);

private:
    std::map<enum KEYTYPE, KeyValue *> keys;
};


struct Led
{
    SDL_Rect coords;
    SDL_Color ledColor;
    std::string description;
};

class Configuration 
{
private:
	YAML::Node yaml;
	std::string title;
	int windowX;
	int windowY;
	std::string openFont;
    std::string background;
    int ticks;
    int frames;

    KeyHandler keys;

    int totalLeds;
    std::vector<Led *> leds;
    bool ledInfo;
    int brightness; 
    SDL_Color ledColor;

    bool setupComplete;

    static Configuration * _config;
	Configuration();

public:
	~Configuration();

    static Configuration * Get();
    static void Destroy();

	bool setup();

    void handleKeys(bool &running);

    void setText(std::string message);
    void renderText(int x, int y, int w, int h);

    void renderBackground();

    void screenClear();
    void screenRender();

    void renderLeds();

    void delay(int override = -1);

    void setTotalLeds (int leds);
    int getTotalLeds() const;

    void setBrightness(int brightness) {
        this->brightness = brightness;
        this->ledColor.a = brightness;
    }

    int getBrightness() const {
        return this->brightness;
    }

    void setPixelColor(int led, int r, int g, int b);

    SDL_Color & getPixelColor() {
        return this->ledColor;
    }

    int getWidth() const {
        return this->windowX;
    }

    int getHeight() const {
        return this->windowY;
    }

    void toggleSetupCompleted(){
        this->setupComplete = !this->setupComplete;
    }

    bool isSetupComplete() const {
        return this->setupComplete;
    }

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	TTF_Font * font;
    SDL_Surface * fontSurface;
    SDL_Texture * fontTexture;
    SDL_Texture * backgroundTexture;
};

#endif