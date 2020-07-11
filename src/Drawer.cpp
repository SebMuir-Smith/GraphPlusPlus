#include <math.h>
#include <valarray>
#include "Drawer.hpp"

Drawer::Drawer(Grapher grapher_in){
    grapher = grapher_in;
}

void Drawer::addplot(double (*functionToGraph)(double), double xStart, double xEnd, int resolution) 
{
    if (&grapher == nullptr){
        std::cerr << "ERROR";
        return;
    }
    int memsize = resolution; /* ceil((xEnd - xStart) * resolution); /* Round up to find how many x vals we need at this resolution */

    double iterator = (xEnd - xStart) / memsize;
    double xVals[memsize];

    for (int i = 0; i < memsize; i++)
    {
        xVals[i] = xStart + i * iterator;
    }

    std::valarray<double> x(xVals, memsize);
    std::valarray<double> y = x.apply(functionToGraph);

    double min = INFINITY;
    double max = -INFINITY;

    for (int i = 1; i < memsize; i++)
    {
        min = y[i] < min ? y[i] : min;
        max = y[i] > max ? y[i] : max;
        grapher.lineGraphFunction(round(x[i - 1]), round(y[i - 1]), round(x[i]), round(y[i]));
    }
}

double testFunc(double x){
        return x;
}

int main(int nargs, char **args)
{
    Grapher grapher;

    int result = grapher.setupScreen();
    if (result){
        return result;
    }

    Drawer drawer(grapher);
    std::cout << "Starting...";

    SDL_RenderDrawLine(grapher.renderer, 1,100,100,200);
    SDL_RenderDrawLine(grapher.renderer, 100,200,200,300);
    
    drawer.addplot(testFunc, 1, 1000, 10);

    std::cout << SDL_GetError();
    grapher.holdUntilQuit();

    return 0;
}