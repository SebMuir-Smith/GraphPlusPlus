#include <math.h>
#include "Line.hpp"
using namespace std;

Line::Line()
{
    r_colour = 0;
    g_colour = 0;
    b_colour = 0;
    xStart = 0;
    xEnd = 1;
    yMin = INFINITY;
    yMax = -INFINITY;

    // By default scale to screen
    xScale = 100;
    xAdj = 0;
    yScale = 100;
    yAdj = 0;
}

Line* Line::addBounds(double xStart_in, double xEnd_in)
{
    xStart = xStart_in;
    xEnd = xEnd_in;

    return this;
}

Line* Line::addFunction(double (*functionToGraph_in)(double))
{
    functionToGraph = functionToGraph_in;

    return this;
}

Line* Line::addColour(uint8_t r, uint8_t g, uint8_t b, uint8_t o)
{
    r_colour = r;
    g_colour = g;
    b_colour = b;
    o_colour = o;
    return this;
}

Line* Line::scaleTo(int xSize, int ySize, int xOffset, int yOffset)
{
    xAdj = xOffset;
    xScale = xSize;

    yAdj = yOffset;
    yScale = ySize;

    return this;
}

Line* Line::updatePlot()
{

    if (functionToGraphLastRun != functionToGraph || xStart != xStartLastRun || xEnd != xEndLastRun)
    {
        runFunction();

        /* Scale */
        for (int i = 0; i < GRAPHING_ITERATIONS; i++)
        {
            points[i].x = (points[i].x - xStart) * (xScale / (xEnd - xStart)) + xAdj;
            points[i].y = (points[i].y - yMin) * (yScale / (yMax - yMin)) + yAdj;

            //std::cout << "Now Drawing x = " << x[i] << " to y = " << y[i] << std::endl;
            /*
            if (x[i] > (SCREEN_WIDTH + 1) || x[i] < 0 || y[i] > (SCREEN_HEIGHT + 1) || y[i] < 0)
            {
                std::cerr << "ERROR: POINT " << i << " IS OUT OF BOUNDS, x = " << x[i] << " y = " << y[i] << "\n";
                throw 2;
            }*/
        }
    }

    //grapher.lineGraphFunction(points, GRAPHING_ITERATIONS, r_colour, g_colour, b_colour, o_colour);

    return this;
}

Line* Line::runFunction()
{
    /* Now run the function and fill in values */
    double iterator = (xEnd - xStart);

    for (int i = 0; i < GRAPHING_ITERATIONS; i++)
    {
        points[i].x = xStart + (iterator * i) / (GRAPHING_ITERATIONS - 1);
        points[i].y = -functionToGraph(points[i].x);

        yMin = points[i].y < yMin ? points[i].y : yMin;
        yMax = points[i].y > yMax ? points[i].y : yMax;
    }

    xStartLastRun = xStart;
    xEndLastRun = xEnd;
    functionToGraphLastRun = functionToGraph;

    return this;
}
