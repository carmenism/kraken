#ifndef _INSTRUCTIONALWIDGET_H
#define _INSTRUCTIONALWIDGET_H

#include <QtGui/QWidget>
#include <QString>

class InstructionalWidget : public QWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    InstructionalWidget(const QString & message, QWidget *parent = NULL);
    ~InstructionalWidget();
    
};

#endif