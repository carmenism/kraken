#ifndef STATISTICS_H_
#define STATISTICS_H_

class LineChart;

#include <QList>

class Statistics {
public:
    Statistics(LineChart *chart);
    ~Statistics();

    void recalculate();

    void draw();
    void drawBoxPlots();
    void drawHurricaneTrack();
    void drawHurricaneTrackBand(QList<double> *top, QList<double> *bottom);

    bool getDisplay() { return display; }
    void setDisplay(bool d) { display = d; }
    void displayOn() { display = true; }
    void displayOff() { display = false; }
    
    bool getDisplayBoxPlots() { return displayBoxPlots; }
    void setDisplayBoxPlots(bool d) { displayBoxPlots = d; }
    void displayBoxPlotsOn() { displayBoxPlots = true; }
    void displayBoxPlotsOff() { displayBoxPlots = false; }
    
    bool getDisplayHurricaneTrack() { return displayHurricaneTrack; }
    void setDisplayHurricaneTrack(bool d) { displayHurricaneTrack = d; }
    void displayHurricaneTrackOn() { displayHurricaneTrack = true; }
    void displayHurricaneTrackOff() { displayHurricaneTrack = false; }
private:
    bool display, displayBoxPlots, displayHurricaneTrack;
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

    void findQuartiles(QList<double> list);
    double findMedian(QList<double> sortedList, int indexStart, int indexEnd);

    float calculateXLocation(float valueX);
    float calculateYLocation(float valueY);
};

#endif /* STATISTICS_H_ */