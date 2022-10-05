#include "configuration.h"

#include <algorithm>

using namespace YAML;
const char * CONFIGURATION_FILE = "config.yaml";

Configuration * Configuration::_config = NULL;

template<class T>
class deleter
{
public:
    void operator()(const T * ptr) const
    {
        delete ptr;
    }
};

Led::Led()
{

}

Led::~Led()
{

}

Configuration * Configuration::Get() 
{
    if (_config == NULL) {
        _config = new Configuration;
    }
    return _config;
}

void Configuration::Destroy()
{
    printf("Closing shop...\n");
    if (_config != NULL)
    {
        delete _config;
    }
}

Configuration::Configuration():
    yaml(YAML::LoadFile(CONFIGURATION_FILE)),
    title("main"),
    totalLeds(0),
    brightness(0),
    window(NULL),
    renderer(NULL),
    font(NULL),
    fontSurface(NULL),
    fontTexture(NULL)
{
    title = yaml["title"].as<std::string>();
    windowX = yaml["window-x"].as<int>();
    windowY = yaml["window-y"].as<int>();
    ticks = yaml["ticks"].as<int>();
    frames = yaml["frames"].as<int>();
    openFont = yaml["ttf-font"].as<std::string>();
}
Configuration::~Configuration() 
{
    if (!this->leds.empty()){
        // Delete all
        std::for_each(this->leds.begin(), this->leds.end(), deleter<Led>());
    }
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    if (font != NULL){
        TTF_CloseFont(font);
    }
    if (fontSurface != NULL){
        SDL_FreeSurface(fontSurface);
        SDL_DestroyTexture(fontTexture);
    }

    TTF_Quit();
    SDL_Quit();
}

bool Configuration::setup(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing sdl. %s\n", SDL_GetError());
        return false;
    }
    /* Create window */
    if(!(window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, 0)))
    {
        printf("Error creating window: %s\n", SDL_GetError());
        return false;
    }
    /* Create a renderer */
    if(!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
    {
        printf("Error creating renderer: %s\n", SDL_GetError());
        return false;
    }

    /* Create font */
    TTF_Init();
    if(!(font = TTF_OpenFont(openFont.c_str(), 25)))
    {
        printf("Error creating font: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void Configuration::createText(std::string message)
{
    SDL_Color white = { 255, 255, 255 };
    // Release surface before reusing
    if (fontSurface != NULL){
        SDL_FreeSurface(fontSurface);
    }
    fontSurface = TTF_RenderText_Solid(font, message.c_str(), white);
    // Same, if used before release
    if (fontTexture != NULL){
        SDL_DestroyTexture(fontTexture);
    }
    fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
}

void Configuration::renderText()
{
    SDL_Rect rect = { 0,0,windowX/2, windowY/4 };
    SDL_RenderCopy(renderer, fontTexture, NULL, &rect);
}

void Configuration::screenClear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Configuration::screenRender()
{
    SDL_RenderPresent(renderer);
}

void Configuration::renderLeds() 
{
    if (leds.empty()){
        return;
    }
    
    SDL_SetRenderDrawColor(renderer, ledColor.r, ledColor.g, ledColor.b, 255);
    // printf("r: %d g: %d b: %d", ledColor.r, ledColor.g, ledColor.b);
    for (int i = 0; i < totalLeds; i++)
    {
        SDL_RenderFillRect(renderer, &leds[i]->coords);
    }
}

void Configuration::delay(int override)
{
    if (override != 1)
    {
        SDL_Delay(override/frames);
    } else {
        SDL_Delay(ticks/frames);
    }

}

void Configuration::setTotalLeds (int leds) 
{
    this->totalLeds = leds;
    if (!this->leds.empty()){
        // Delete all
        std::for_each(this->leds.begin(), this->leds.end(), deleter<Led>());
    }
    // Create leds
    for (int i = 0; i < leds; i++){
        Led * led = new Led();
        const std::string currentLed = "led" + std::to_string(i);
        led->coords.x = yaml[currentLed]["position"]["x"].as<int>();
        led->coords.y = yaml[currentLed]["position"]["y"].as<int>();
        led->coords.w = yaml[currentLed]["position"]["width"].as<int>();
        led->coords.h = yaml[currentLed]["position"]["height"].as<int>();
        // printf("Got led x: %d y: %d w: %d h: %d\n", led->coords.x, led->coords.y, led->coords.w, led->coords.h);
    
        this->leds.push_back(led);
    }
}

int Configuration::getTotalLeds() const
{
    return this->totalLeds;
}

void Configuration::setPixelColor(int r, int g, int b){
    if (this->leds.empty()){
        return;
    }
    ledColor.r = r;
    ledColor.g = g;
    ledColor.b = b;
}