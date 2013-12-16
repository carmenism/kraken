#ifndef STATISTICS_H_
#define STATISTICS_H_

class LineChart;
class Color;

#include <QList>

class Statistics {
public:
    Statistics(LineChart *chart);
    ~Statistics();

    void recalculate();

    void draw();

    bool getDisplayErrorBars() { return displayErrorBars; }
    void setDisplayErrorBars(bool d) { displayErrorBars = d; }
    void displayErrorBarsOn() { displayErrorBars = true; }
    void displayErrorBarsOff() { displayErrorBars = false; }
    
    bool getDisplayBoxPlots() { return displayBoxPlots; }
    void setDisplayBoxPlots(bool d) { displayBoxPlots = d; }
    void displayBoxPlotsOn() { displayBoxPlots = true; }
    void displayBoxPlotsOff() { displayBoxPlots = false; }
    
    bool getDisplayErrorBands() { return displayErrorBands; }
    void setDisplayErrorBands(bool d) { displayErrorBands = d; }
    void displayErrorBandsOn() { displayErrorBands = true; }
    void displayErrorBandsOff() { displayErrorBands = false; }

    void useQuartiles();
    void useStandardDeviations();

    bool getUsingQuartiles();
    bool getUsingStandardDeviations();
private:
    bool displayBoxPlots, displayErrorBands;
    bool displayErrorBars;
    int startIndex;
    int interval;

    LineChart *chart;

    QList<double> *mean;   // middle line
    QList<double> *stdDev; // box boundaries
    QList<double> *min;    // whisker bottom
    QList<double> *max;    // whisker end
    QList<double> *median; 
    QList<double> *q1;
    QList<double> *q3;

    QList<double> *meanPlus2SD; 
    QList<double> *meanPlus1SD; 
    QList<double> *meanMinus1SD;
    QList<double> *meanMinus2SD; 

    QList<double> *insideTop;
    QList<double> *insideBottom;
    QList<double> *outsideTop;
    QList<double> *outsideBottom;

    void drawErrorBars();
    void drawBoxPlots();
    void drawErrorBands();
    void drawErrorBand(QList<double> *top, QList<double> *bottom, Color *color, float alpha);

    void findQuartiles(QList<double> list);
    double findMedian(QList<double> sortedList, int indexStart, int indexEnd);

    float calculateXLocation(float valueX);
    float calculateYLocation(float valueY);

    void drawLine(QList<double> *list, double lineWidth);
};

#endif /* STATISTICS_H_ */