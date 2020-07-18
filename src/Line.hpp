#include <SDL2/SDL.h>
#define GRAPHING_ITERATIONS 100

class Line
{
public:
    Line();
    Line addFunction(double (*functionToGraph)(double));
    Line updatePlot();
    Line scaleTo(int xSize, int ySize, int xOffset = 0, int yOffset = 0);
    Line addBounds(double xStart, double xEnd);
    Line addColour(uint8_t r, uint8_t g, uint8_t b, uint8_t o = 255);
    uint8_t r_colour;
    uint8_t g_colour;
    uint8_t b_colour;
    uint8_t o_colour;
    SDL_FPoint points[GRAPHING_ITERATIONS];
private:
    Line runFunction();
    double (*functionToGraph)(double);
    int xStart;
    int xEnd;
    int xStartLastRun;
    int xEndLastRun;
    double (*functionToGraphLastRun)(double);
    double yMin;
    double yMax;
    double yAdj;
    double yScale;
    double xAdj;
    double xScale;
};
