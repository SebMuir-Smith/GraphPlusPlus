#include "Grapher.hpp"

class Drawer
{
public:
    void addplot(double (*functionToGraph)(double), double xStart, double xEnd, int resolution);
    Drawer(Grapher grapher_in);
private:
    Grapher grapher;
};
