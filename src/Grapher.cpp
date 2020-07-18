#include <math.h>
#include "Grapher.hpp"
#ifndef CLEANUP_H
#define CLEANUP_H

#include <utility>

/*
 * Recurse through the list of arguments to clean up, cleaning up
 * the first one in the list each iteration.
 */
template <typename T, typename... Args>
void cleanup(T *t, Args &&... args)
{
    //Cleanup the first item in the list
    cleanup(t);
    //Recurse to clean up the remaining arguments
    cleanup(std::forward<Args>(args)...);
}
/*
 * These specializations serve to free the passed argument and also provide the
 * base cases for the recursive call above, eg. when args is only a single item
 * one of the specializations below will be called by
 * cleanup(std::forward<Args>(args)...), ending the recursion
 * We also make it safe to pass nullptrs to handle situations where we
 * don't want to bother finding out which values failed to load (and thus are null)
 * but rather just want to clean everything up and let cleanup sort it out
 */
template <>
inline void cleanup<SDL_Window>(SDL_Window *win)
{
    if (!win)
    {
        return;
    }
    SDL_DestroyWindow(win);
}
template <>
inline void cleanup<SDL_Renderer>(SDL_Renderer *ren)
{
    if (!ren)
    {
        return;
    }
    SDL_DestroyRenderer(ren);
}
template <>
inline void cleanup<SDL_Texture>(SDL_Texture *tex)
{
    if (!tex)
    {
        return;
    }
    SDL_DestroyTexture(tex);
}
template <>
inline void cleanup<SDL_Surface>(SDL_Surface *surf)
{
    if (!surf)
    {
        return;
    }
    SDL_FreeSurface(surf);
}

#endif

/**
        * Log an SDL error with some error message to the output stream of our choice
        * @param os The output stream to write the message to
        * @param msg The error message to write, format will be msg error: SDL_GetError()
        */
void Grapher::logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

void Grapher::holdUntilMouse()
{
    SDL_RenderPresent(renderer);
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                quit = true;
            }
        }
    }
}

void Grapher::holdUntilQuit()
{
    SDL_RenderPresent(renderer);
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
    }

    SDL_Quit();
}

/**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture *Grapher::loadTexture(const std::string &file, SDL_Renderer *ren)
{
    //Initialize to nullptr to avoid dangling pointer issues
    SDL_Texture *texture = nullptr;
    //Load the image
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    //If the loading went ok, convert to texture and return the texture
    if (loadedImage != nullptr)
    {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        //Make sure converting went ok too
        if (texture == nullptr)
        {
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else
    {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
void Grapher::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    //Query the texture to get its width and height to use
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

int Grapher::setupScreen()
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Lesson 2", 100, 100, SCREEN_WIDTH,
                                          SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }
    /*
            SDL_Texture *background = loadTexture(resPath + "background.bmp", renderer);
            SDL_Texture *image = loadTexture(resPath + "hello.bmp", renderer);
            if (background == nullptr || image == nullptr)
            {
                cleanup(background, image, renderer, window);
                SDL_Quit();
                return 1;
            }
            */

    // Select the color for drawing. It is set to red here.
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    return 0;
}

void Grapher::lineGraphFunction(SDL_FPoint *points, int nPoints, uint8_t r, uint8_t g, uint8_t b, uint8_t o)
{

    uint8_t _r, _g, _b, _o;

    SDL_GetRenderDrawColor(renderer, &_r, &_g, &_b, &_o);
    SDL_SetRenderDrawColor(renderer, r, g, b, o);
    int result = SDL_RenderDrawLinesF(renderer, points, nPoints);
    SDL_SetRenderDrawColor(renderer, _r, _g, _b, _o);

    if (result != 0){
        std::cout << "error" << result << SDL_GetError() << std::endl;
        abort();
    }

}

void Grapher::addLine(Line* line){
    line_pointers.push_front(line);
}

void Grapher::updateLines(){
    for (Line* line_pointer : line_pointers) {
        line_pointer->updatePlot();
        lineGraphFunction(line_pointer->points,
         GRAPHING_ITERATIONS, line_pointer->r_colour, line_pointer->g_colour, line_pointer->b_colour, line_pointer->o_colour);
    }
}

double testFunc(double x)
{
    return x * x;
}

double testFunc2(double x)
{
    return x < 10 ? x : x * 2;
}

double testFunc3(double x)
{
    return x * x * x;
}

double testFunc4(double x)
{
    return tan(x) > 0 ? std::min(tan(x), (double)2.0) : std::max(tan(x), -2.0);
}


int main(int nargs, char **args)
{
    Grapher grapher;

    int result = grapher.setupScreen();
    if (result)
    {
        return result;
    }

    Line cosLine;
    Line sinLine;

    cosLine = cosLine.addBounds(0,3).scaleTo(grapher.SCREEN_WIDTH, grapher.SCREEN_HEIGHT).addFunction(cos);
    sinLine = sinLine.addBounds(0,3).scaleTo(grapher.SCREEN_WIDTH, grapher.SCREEN_HEIGHT).addFunction(sin);

    grapher.addLine(&cosLine);
    grapher.addLine(&sinLine);
    grapher.updateLines();
    grapher.holdUntilMouse();

    for (int i = 1; i < 255; i++)
    {
        SDL_SetRenderDrawColor(grapher.renderer, 255, 255, 255, 255);
        SDL_RenderClear(grapher.renderer);
        //cosLine = cosLine.addBounds(((double) i)/10,3 + ((double) i)/10).addColour(0,i,0);
        //sinLine = sinLine.addBounds(((double) i)/10,3 + ((double) i)/10).addColour(0,0,i);
        grapher.updateLines();
        SDL_RenderPresent(grapher.renderer);
        std::cout << i;
    }

    grapher.holdUntilQuit();

    return 0;
}


