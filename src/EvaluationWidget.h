#ifndef _EVALUATIONWIDGET_H
#define _EVALUATIONWIDGET_H

#include <QtGui/QWidget>
#include <QString>
#include <fstream>
#include <cstdio>

class MyQGLWidget;
class QFormLayout;
class QStackedWidget;
class QDialogButtonBox;
class QLabel;
class QWidget;

class EvaluationWidget : public QWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    EvaluationWidget(MyQGLWidget *myQGLWidget, QWidget *parent = NULL);
    ~EvaluationWidget();
    
    public slots:
        void accept();
        void accepted();
private:
    FILE * outFile;
    std::string filename;
    MyQGLWidget *myQGLWidget;

    QFormLayout *layout;
    QStackedWidget *stackedWidget;
    QDialogButtonBox *buttonBox;
    QLabel *warningMessage;

    void makeQuestion(const QString & question);
    void makeInstruction(const QString & message);

    QWidget *getCurrentWidget();
    bool isAdvancable();
    void advancePage();

    void determineFilename(QString initials);

    void openFile();
    void closeFile();
    void writeToFile(std::string line);
};

#endif