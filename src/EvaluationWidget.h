#ifndef _EVALUATIONWIDGET_H
#define _EVALUATIONWIDGET_H

#include <QtGui/QWidget>
#include <QString>

class MyQGLWidget;
class QFormLayout;
class QStackedWidget;
class QDialogButtonBox;
class QLabel;

class EvaluationWidget : public QWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    EvaluationWidget(MyQGLWidget *myQGLWidget, QWidget *parent = NULL);
    ~EvaluationWidget();
    
    public slots:
        void accept();
        void accepted();
private:
    MyQGLWidget *myQGLWidget;

    QFormLayout *layout;
    QStackedWidget *stackedWidget;
    QDialogButtonBox *buttonBox;
    QLabel *warningMessage;

    void makeQuestion(const QString & question);
};

#endif