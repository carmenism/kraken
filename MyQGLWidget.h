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

    void updateLineChart(QList<QList<double>> matrix, QStringList labels); 

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
    void selectItem(int x, int y);
    void setHovered(ChartPoint *point);

    MS_PROD_MainWindow *mainWindow;
    std::vector<MultiSpeciesLineChart *> charts;
    
    std::vector<ChangeSlider *> sliders;
    ChartPoint *hovered;

    std::string labelSuffix;
};

#endif  /* _MYQGLWIDGET_H */