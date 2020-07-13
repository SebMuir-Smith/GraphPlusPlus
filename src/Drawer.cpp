#include <math.h>
#include <valarray>
#include "Drawer.hpp"

Drawer::Drawer(Grapher grapher_in){
    grapher = grapher_in;
}

void Drawer::addplot(double (*functionToGraph)(double), double xStart, double xEnd, int resolution, int screenWitdth, int screenHeight) 
{

    if (&grapher == nullptr){
        std::cerr << "ERROR";
        return;
    }
    int memSize = resolution; /* ceil((xEnd - xStart) * resolution); /* Round up to find how many x vals we need at this resolution */

    double iterator = (xEnd - xStart) / memSize;
    double xVals[memSize];

    for (int i = 0; i < memSize; i++)
    {
        xVals[i] = xStart + i * iterator;
    }

    std::valarray<double> x(xVals, memSize);
    std::valarray<double> y = x.apply(functionToGraph);

    double min = INFINITY;
    double max = -INFINITY;
    double sum = 0;

    for (int i = 1; i < memSize; i++)
    {
        min = y[i] < min ? y[i] : min;
        max = y[i] > max ? y[i] : max;
        sum += y[i];
    }

    double x_multiplier = screenWitdth / (xEnd - xStart) ;
    double y_multiplier = screenHeight / (max - min) ;
    double y_adj = screenHeight/(2 * y_multiplier) - sum / memSize ; /* average */

    for (int i = 1; i < memSize; i++)
    {
    grapher.lineGraphFunction(x[i] * x_multiplier, (y[i] + y_adj) * y_multiplier, memSize -1);
    }
}

double testFunc(double x){
        return x*x;
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

    /*SDL_RenderDrawLine(grapher.renderer, 1,100,100,200);
    SDL_RenderDrawLine(grapher.renderer, 100,200,200,300);*/
    
    drawer.addplot(testFunc, 1, 100, 1000, grapher.SCREEN_WIDTH, grapher.SCREEN_HEIGHT);

    std::cout << SDL_GetError();
    grapher.holdUntilQuit();

    return 0;
}