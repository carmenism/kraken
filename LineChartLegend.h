#ifndef LINECHARTLEGEND_H_
#define LINECHARTLEGEND_H_

class LineChart;

class LineChartLegend {
public:
    LineChartLegend(LineChart *c);
    ~LineChartLegend();

    void draw(float x, float y, float lineLength, float spacing, void *font);

    float getFontHeight() { return fontHeight; }
    void setFontHeight(float h) { fontHeight = h; }
private:
    LineChart *chart;

    void drawBoundary(float width, float height);

    float fontHeight;
};

#endif /* LINECHARTLEGEND_H_ */