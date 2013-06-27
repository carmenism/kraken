#ifndef GRAPHLEGEND_H
#define GRAPHLEGEND_H

class LineChart;

class GraphLegend {
public:
    GraphLegend(LineChart *c);
    ~GraphLegend();

    void draw(float x, float y, float lineLength, float spacing, void *font);
private:
    LineChart *chart;

    void drawBoundary(float width, float height);
};

#endif /* GRAPHLEGEND_H */