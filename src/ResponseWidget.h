#ifndef _RESPONSEWIDGET_H
#define _RESPONSEWIDGET_H

#include <QtGui/QWidget>
#include <QString>
#include <string>

class ResponseWidget : public QWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    ResponseWidget(QWidget *parent = NULL) : QWidget(parent) {}
    ~ResponseWidget() {}
    
    virtual bool completed() = 0;
    virtual std::string getLine() = 0;
};

#endif