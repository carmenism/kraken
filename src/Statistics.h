#ifndef STATISTICS_H_
#define STATISTICS_H_

class LineChart;

#include <QList>

class Statistics {
public:
    Statistics(LineChart *chart);
    ~Statistics();

    void recalculate();

    void print();

    void draw();

    bool getDisplay() { return display; }
    void setDisplay(bool d) { display = d; }
    void displayOn() { display = true; }
    void displayOff() { display = false; }
private:
    bool display;
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

    QList<double> *whiskerTop;
    QList<double> *whiskerBottom;
    
    QList<double> *boxTop;
    QList<double> *boxBottom;
    
    QList<double> *boxMiddle;

    void findQuartiles(QList<double> list);
    double findMedian(QList<double> sortedList, int indexStart, int indexEnd);

    float calculateXLocation(float valueX);
    float calculateYLocation(float valueY);
};

#endif /* STATISTICS_H_ */