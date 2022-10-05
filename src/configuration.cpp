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
    ledInfo(true),
    brightness(0),
    window(NULL),
    renderer(NULL),
    font(NULL),
    fontSurface(NULL),
    fontTexture(NULL),
    backgroundTexture(NULL)
{
    title = yaml["title"].as<std::string>();
    windowX = yaml["window-x"].as<int>();
    windowY = yaml["window-y"].as<int>();
    ticks = yaml["ticks"].as<int>();
    frames = yaml["frames"].as<int>();
    ledInfo = yaml["led-info"].as<bool>();
    openFont = yaml["ttf-font"].as<std::string>();
    background = yaml["background"].as<std::string>();

    // Setup keys
    keys[NAVIGATION] = SDLK_a;
    keys[NAVIGATION_UP] = SDLK_a;
    keys[NAVIGATION_DOWN] = SDLK_a;
    keys[UP] = SDLK_UP;
    keys[DOWN] = SDLK_DOWN;
    keys[LEFT] = SDLK_LEFT;
    keys[RIGHT] = SDLK_RIGHT;
    keys[SELECT] = SDLK_TAB;
    keys[START] = SDLK_RETURN;
    keys[A] = SDLK_a;
    keys[B] = SDLK_b;
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

    if (backgroundTexture != NULL){
        SDL_DestroyTexture(backgroundTexture);
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

    /* Create background */
    if (!(backgroundTexture = IMG_LoadTexture(renderer, background.c_str())))
    {
        printf("Couldn't load background image: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

void Configuration::handleKeys(bool &running) {
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                break;
            default:
                break;
        }
    }
}

void Configuration::setText(std::string message)
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

void Configuration::renderText(int x, int y, int w, int h)
{
    SDL_Rect rect = { x,y,w,h };
    SDL_RenderCopy(renderer, fontTexture, NULL, &rect);
}

void Configuration::renderBackground()
{
    int width=0, height=0;
    SDL_QueryTexture(backgroundTexture, NULL, NULL, &width, &height);
    //SDL_Rect rect = { 0,windowY-height,width, height };
    SDL_Rect rect = { 0, 0, width, height };
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &rect);
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
    
    //SDL_SetRenderDrawColor(renderer, ledColor.r, ledColor.g, ledColor.b, 255);
    // printf("r: %d g: %d b: %d", ledColor.r, ledColor.g, ledColor.b);
    for (int i = 0; i < totalLeds; i++)
    {
        // Render leds if enabled
        if (ledInfo){
            setText(leds[i]->description);
            renderText(leds[i]->coords.x, leds[i]->coords.y - 30, leds[i]->coords.w, 25);
        }
        SDL_SetRenderDrawColor(renderer, leds[i]->ledColor.r, leds[i]->ledColor.g, leds[i]->ledColor.b, ledColor.a);
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
        led->description = yaml[currentLed]["description"].as<std::string>();
        // printf("Got led x: %d y: %d w: %d h: %d\n", led->coords.x, led->coords.y, led->coords.w, led->coords.h);
    
        this->leds.push_back(led);
    }
}

int Configuration::getTotalLeds() const
{
    return this->totalLeds;
}

void Configuration::setPixelColor(int led, int r, int g, int b){
    if (this->leds.empty()){
        return;
    }
    ledColor.r = r;
    ledColor.g = g;
    ledColor.b = b;
    this->leds[led]->ledColor.r = r;
    this->leds[led]->ledColor.g = g;
    this->leds[led]->ledColor.b = b;
}