#include <SDL2/SDL_image.h>
#include <cstdio>
#include "graphics.hpp"


graphics::graphics()
{
    //Todo: Error check
    window = SDL_CreateWindow("Avatool", 0, 0, 1280, 720, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    
    sharedFont = new systemFont();
    texManager = new textureManager();
}

graphics::~graphics()
{
    delete texManager;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void graphics::beginFrame(uint32_t clearColor)
{
    clearTexture(NULL, COLOR_DEFAULT_CLEAR);
}

void graphics::endFrame()
{
    SDL_RenderPresent(renderer);
}

void graphics::renderTextf(SDL_Texture *target, int fontSize, uint32_t color, int x, int y, const char *format, ...)
{
    char str[2048];
    va_list args;
    va_start(args, format);
    vsprintf(str, format, args);
    va_end(args);
    sharedFont->renderText(renderer, target, fontSize, color, x, y, str);
}

void graphics::renderTextfWrap(SDL_Texture *target, int fontSize, uint32_t color, int x, int y, int maxWidth, const char *format, ...)
{
    char str[2048];
    va_list args;
    va_start(args, format);
    vsprintf(str, format, args);
    va_end(args);
    sharedFont->renderTextWrap(renderer, target, fontSize, x, y, maxWidth, color, str);
}

void graphics::clearTexture(SDL_Texture *texture, uint32_t color)
{
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, getRed(color), getGreen(color), getBlue(color), getAlpha(color));
    SDL_RenderClear(renderer);
}

void graphics::renderTexture(SDL_Texture *texture, SDL_Texture *target, int x, int y)
{
    //Get source width and height
    int textureWidth = 0, textureHeight = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect sourceRect = { 0, 0, textureWidth, textureHeight };
    SDL_Rect destinationRect = { x, y, textureWidth, textureHeight };

    SDL_SetRenderTarget(renderer, target);
    SDL_RenderCopy(renderer, texture, &sourceRect, &destinationRect);
}

void graphics::renderTexturePart(SDL_Texture *texture, SDL_Texture *target, int x, int y, int sourceX, int sourceY, int sourceWidth, int sourceHeight)
{
    SDL_Rect sourceRect = { sourceX, sourceY, sourceWidth, sourceHeight };
    SDL_Rect destinationRect = { x, y, sourceWidth, sourceHeight };

    SDL_SetRenderTarget(renderer, target);
    SDL_RenderCopy(renderer, texture, &sourceRect, &destinationRect);
}

void graphics::renderTextureStretched(SDL_Texture *texture, SDL_Texture *target, int x, int y, int width, int height)
{
    int textureWidth = 0, textureHeight = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect sourceRect = { 0, 0, textureWidth, textureHeight };
    SDL_Rect destinationRect = { x, y, width, height };

    SDL_SetRenderTarget(renderer, target);
    SDL_RenderCopy(renderer, texture, &sourceRect, &destinationRect);
}

void graphics::renderLine(SDL_Texture *target, uint32_t color, int x1, int y1, int x2, int y2)
{
    SDL_SetRenderTarget(renderer, target);
    SDL_SetRenderDrawColor(renderer, getRed(color), getGreen(color), getBlue(color), getAlpha(color));
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void graphics::renderRectFill(SDL_Texture *target, uint32_t color, int x, int y, int width, int height)
{
    SDL_SetRenderTarget(renderer, target);
    SDL_SetRenderDrawColor(renderer, getRed(color), getGreen(color), getBlue(color), getAlpha(color));

    SDL_Rect rectRect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &rectRect);
}
