#ifndef _MYQGLWIDGET_H
#define _MYQGLWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <vector>

class Square;
class Circle;
class Triangle;
class MultiSpeciesLineChart;
class ChartPoint;
class ChangeSlider;
class Parameters;
class UndoButton;
class ResetButton;
class MS_PROD_MainWindow;


typedef std::vector<MultiSpeciesLineChart *> MSLineChartList;
typedef std::vector<MultiSpeciesLineChart *>::const_iterator MSLineChartIterator;

#define FOREACH_MSLINECHART(it, msLineChartList) \
    for(MSLineChartIterator it = msLineChartList.begin(); it != msLineChartList.end(); ++it)
#define FOREACH_MSLINECHARTP(it, msLineChartList) \
    for(MSLineChartIterator it = msLineChartList->begin(); it != msLineChartList->end(); ++it)


class MyQGLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    MyQGLWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent = NULL);

    void updateCharts(QList<QList<double>> matrix, QStringList labels); 

    void initializeSliders();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    std::vector<UndoButton *> undoButtons;
    std::vector<ResetButton *> resetButtons;
    void setHovered(ChartPoint *point);

    MS_PROD_MainWindow *mainWindow;
    std::vector<MultiSpeciesLineChart *> charts;
    
    std::vector<ChangeSlider *> sliders;
    ChartPoint *hovered;

    std::string labelSuffix;

    bool mouseReleaseButtons(float x, float y);

    bool mousePressButtons(float x, float y);
    bool mousePressSliders(float x, float y);

    bool mouseMoveButtons(float x, float y);
    bool mouseMoveSliders(float x, float y);
    void mouseMoveChartPoints(int x, int y);

    void initializeCharts(QList<QList<double>> matrix, QStringList labels);

};

#endif  /* _MYQGLWIDGET_H */