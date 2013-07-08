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
    MultiSpeciesLineChart *chart;
    
    std::vector<ChangeSlider *> sliders;
    ChartPoint *hovered;
};

#endif  /* _MYQGLWIDGET_H */