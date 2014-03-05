#ifndef _EVALUATIONWIDGET_H
#define _EVALUATIONWIDGET_H

#include <QtGui/QWidget>
#include <QString>

class MS_PROD_MainWindow;
class QFormLayout;
class QStackedWidget;
class QDialogButtonBox;

class EvaluationWidget : public QWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    EvaluationWidget(MS_PROD_MainWindow *mainWindow, QWidget *parent = NULL);
    ~EvaluationWidget();
    
    public slots:
        void accept();
        void accepted();
private:
    QFormLayout *layout;
    QStackedWidget *stackedWidget;
    QDialogButtonBox *buttonBox;

    void makeQuestion(const QString & question);
};

#endif