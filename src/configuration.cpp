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

TextHandler::TextHandler()
{

}
TextHandler::~TextHandler()
{
    for(std::map<std::string, text>::iterator i = collection.begin(); i != collection.end(); i++)
    {
        text & t = i->second;
        SDL_FreeSurface(t.fontSurface);
        SDL_DestroyTexture(t.fontTexture);
    }
}
bool TextHandler::setOpenFont(const std::string & openFont)
{
    this->openFont = openFont;
    
    TTF_Init();
    if(!(this->font = TTF_OpenFont(this->openFont.c_str(), 25)))
    {
        return false;
    }
    return true;
}
void TextHandler::renderText(const std::string & message, SDL_Renderer * renderer, SDL_Rect & coords)
{
    if (collection.find(message) == collection.end()){
        SDL_Color white = { 255, 255, 255 };
        SDL_Surface * surface = TTF_RenderText_Solid(font, message.c_str(), white);
        SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
        collection[message] = text { surface, texture };
    }
    const text & t = collection.at(message);
    SDL_RenderCopy(renderer, t.fontTexture, NULL, &coords);
}


KeyValue::KeyValue(enum KEYTYPE type):
down(false)
{
    this->value = this->type = type;
}
KeyValue::KeyValue(enum KEYTYPE type, int value):
down(false)
{
    this->type = type;
    this->value = value;
}

KeyValue::~KeyValue()
{

}

KeyHandler::KeyHandler()
{
    // Setup key defaults
    keys[NAVIGATION] = new KeyValue(NAVIGATION);
    keys[NAVIGATION_UP] = new KeyValue(NAVIGATION_UP);
    keys[NAVIGATION_DOWN] = new KeyValue(NAVIGATION_DOWN);
    keys[UP] = new KeyValue(UP);
    keys[DOWN] = new KeyValue(DOWN);
    keys[LEFT] = new KeyValue(LEFT);
    keys[RIGHT] = new KeyValue(RIGHT);
    keys[SELECT] = new KeyValue(SELECT);
    keys[START] = new KeyValue(START);
    keys[A] = new KeyValue(A);
    keys[B] = new KeyValue(B);
}

KeyHandler::~KeyHandler()
{
    for(std::map<KEYTYPE, KeyValue *>::iterator i = keys.begin(); i != keys.end(); i++)
    {
        delete (i->second);
    }
}

void KeyHandler::setKey(enum KEYTYPE type, int value, int pin)
{
    keys[type]->updateValue(value);
}

KeyValue * KeyHandler::getKey(enum KEYTYPE type)
{
    return keys[type];
}

int KeyHandler::getKeyValue(enum KEYTYPE type)
{
    return keys[type]->getValue();
}

void KeyHandler::pressKey(enum SDL_Keycode key)
{

}

void KeyHandler::releaseKey(enum SDL_Keycode key)
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
    setupComplete(false),
    window(NULL),
    renderer(NULL),
    backgroundTexture(NULL)
{
    title = yaml["title"].as<std::string>();
    windowX = yaml["window-x"].as<int>();
    windowY = yaml["window-y"].as<int>();
    ticks = yaml["ticks"].as<int>();
    frames = yaml["frames"].as<int>();
    ledInfo = yaml["led-info"].as<bool>();
    background = yaml["background"]["image"].as<std::string>();
    backgroundCoords = SDL_Rect {
        yaml["background"]["position"]["x"].as<int>(),
        yaml["background"]["position"]["y"].as<int>(),
        yaml["background"]["position"]["width"].as<int>(),
        yaml["background"]["position"]["height"].as<int>(),
    };

    // Load keys 
    for (YAML::const_iterator i = yaml["keys"].begin(); i != yaml["keys"].end(); i++){
        // Check if keys are set
        const std::string & key = i->first.as<std::string>();
        YAML::Node value = i->second.as<YAML::Node>();
        if (key == "nav"){
            keys.setKey(NAVIGATION, value["keycode"].as<int>(), value["pin"].as<int>());
        } else if (key == "nav-up")
        {
            keys.setKey(NAVIGATION_UP, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "nav-down")
        {
            keys.setKey(NAVIGATION_DOWN, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "up")
        {
            keys.setKey(UP, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "down")
        {
            keys.setKey(DOWN, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "left")
        {
            keys.setKey(LEFT, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "right")
        {
            keys.setKey(RIGHT, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "select")
        {
            keys.setKey(SELECT, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "start")
        {
            keys.setKey(START, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "a")
        {
            keys.setKey(A, value["keycode"].as<int>(), value["pin"].as<int>());
        }else if (key == "b")
        {
            keys.setKey(B, value["keycode"].as<int>(), value["pin"].as<int>());
        }
    }
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
    if (!messages.setOpenFont(yaml["ttf-font"].as<std::string>())){
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

void Configuration::renderText(std::string message, int x, int y, int w, int h)
{
    SDL_Rect rect = { x,y,w,h };
    messages.renderText(message, renderer, rect);
}

void Configuration::renderBackground()
{
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundCoords);
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
    
    for (int i = 0; i < totalLeds; i++)
    {
        // Render leds if enabled
        if (ledInfo){
            renderText(leds[i]->description, leds[i]->coords.x, leds[i]->coords.y - 30, leds[i]->coords.w, 25);
        }
        SDL_SetRenderDrawColor(renderer, leds[i]->ledColor.r, leds[i]->ledColor.g, leds[i]->ledColor.b, ledColor.a);
        SDL_RenderFillRect(renderer, &leds[i]->coords);
    }
}

void Configuration::delay(int override)
{
    if (override != 1)
    {
        //SDL_Delay(override/frames);
        // for (int i = 0; i < override;i++);
    } else {
        // SDL_Delay(ticks/frames);
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

void Configuration::setBrightness(int brightness) {
    this->brightness = brightness;
    this->ledColor.a = brightness;
    for (std::vector<Led *>::iterator i = leds.begin(); i != leds.end(); i++){
        Led * led = *i;
        led->ledColor.a = brightness;   
    }
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