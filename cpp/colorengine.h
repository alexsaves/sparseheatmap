//
// Created by Alexei White on 2015-11-29.
//

#ifndef NODE_HEATMAP_COLORENGINE_H
#define NODE_HEATMAP_COLORENGINE_H

class Colorengine {
public:
    Colorengine(int);

    ~Colorengine();

    void add_color(char r, char g, char b, char a);

private:
    //Colormodel *models;
    char *data;
    int cindex;
    int colors;
};


#endif //NODE_HEATMAP_COLORENGINE_H
