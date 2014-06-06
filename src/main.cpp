#include <QtGui/QApplication>
#include "ms_prod_mainwindow.h"
#include "MyQGLWidget.h"
#include "stdio.h"

//#if defined(WIN32) && defined(_DEBUG)
//#define _CRTDBG_MAP_ALLOC
//#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
//#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MS_PROD_MainWindow w;
    w.show();

    MyQGLWidget *window = new MyQGLWidget(&w);
    window->resize(800,600);
    
    w.setGLWidget(window);

    int result = a.exec();

    //_CrtDumpMemoryLeaks();

    return result;
}
