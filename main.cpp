#include <QtGui/QApplication>
#include "ms_prod_mainwindow.h"
#include "MyQGLWidget.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MS_PROD_MainWindow w;
    w.show();

    MyQGLWidget window;
    window.resize(800,600);
    window.show();

    std::cout << "TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";


    return a.exec();
}
