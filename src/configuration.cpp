#include "configuration.h"

using namespace YAML;
const std::string CONFIGURATION_FILE = "config.yaml";

Configuration::Configuration():
    yaml(LoadFile(CONFIGURATION_FILE)),
    title("main"),
    window(NULL),
    renderer(NULL),
    font(NULL)
{
    title = yaml["title"].as<std::string>();
    windowX = yaml["window-x"].as<int>();
    windowY = yaml["window-y"].as<int>();
    openFont = yaml["ttf-font"].as<std::string>();
}
Configuration::~Configuration() 
{
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
    if (font != NULL){
        TTF_CloseFont(font);
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