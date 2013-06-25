#ifndef GRAPHLEGEND_H
#define GRAPHLEGEND_H

class LineGraph;

class GraphLegend {
public:
    GraphLegend(LineGraph *g);
    ~GraphLegend();

    void draw(float x, float y, float lineLength, float spacing, void *font);
private:
    LineGraph *graph;

    void drawBoundary(float width, float height);
};

#endif /* GRAPHLEGEND_H */