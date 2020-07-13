#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <memory>

class Grapher
    {
    private:
        int pointN;
        SDL_FPoint *points;
        bool initialised;

        /**
        * Log an SDL error with some error message to the output stream of our choice
        * @param os The output stream to write the message to
        * @param msg The error message to write, format will be msg error: SDL_GetError()
        */
        void logSDLError(std::ostream &os, const std::string &msg);

        /**
* Loads a BMP image into a texture on the rendering device
* @param file The BMP image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
        SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *ren);

        /**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
*/
        void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

    public:
        int setupScreen();

        void lineGraphFunction(double x, double y, int nPoints);

        void holdUntilQuit();

        static const int SCREEN_WIDTH = 640;
        static const int SCREEN_HEIGHT = 480;
        
        SDL_Renderer* renderer;
    };
