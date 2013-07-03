#include <QtGui/QApplication>
#include "ms_prod_mainwindow.h"
#include "MyQGLWidget.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MS_PROD_MainWindow w;
    w.show();

    MyQGLWidget *window = new MyQGLWidget();
    window->resize(800,600);
    window->show();

    w.setGLWidget(window);
    window->setParameters(w.getParameters());

    return a.exec();
}
