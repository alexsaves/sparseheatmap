//
// Created by Alexei White on 2015-11-29.
//

#ifndef NODE_HEATMAP_COLORENGINE_H
#define NODE_HEATMAP_COLORENGINE_H

/**
 * Color
 */
struct Clr
{
    char r, g, b, a;
};

/**
 * Color engine
 */
class Colorengine {
public:
    Colorengine(int);

    ~Colorengine();

    void add_color(int, int, int, int);
    void prepare();
    Clr get_color(int intensity);
    int *finalcolors;
    int *data;
    int colorcount;

private:
    int cindex;
};


#endif //NODE_HEATMAP_COLORENGINE_H
