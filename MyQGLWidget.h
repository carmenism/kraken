#ifndef _MYQGLWIDGET_H
#define _MYQGLWIDGET_H

#include <QtOpenGL/QGLWidget>

class Square;
class Circle;
class Triangle;
class LineChart;
class ChartPoint;

class MyQGLWidget : public QGLWidget {

    Q_OBJECT // must include this if you use Qt signals/slots

public:
    MyQGLWidget(QWidget *parent = NULL);

    void setLineChart(LineChart *c) { chart = c; updateGL(); }
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    void selectItem(int x, int y);
    void setHovered(ChartPoint *point);

    LineChart *chart;
    ChartPoint *hovered;
};

#endif  /* _MYQGLWIDGET_H */