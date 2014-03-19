#ifndef _EVALUATIONWIDGET_H
#define _EVALUATIONWIDGET_H

#include <QtGui/QWidget>
#include <QString>
#include <QTime>
#include <fstream>
#include <string>
#include <cstdio>

class MyQGLWidget;
class QFormLayout;
class QStackedWidget;
class QDialogButtonBox;
class QLabel;
class QWidget;
class QAbstractButton;
class QButtonGroup;
class QPushButton;

class EvaluationWidget : public QWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

enum Condition {COND_A, COND_B, COND_C, COND_D};

public:
    EvaluationWidget(MyQGLWidget *myQGLWidget, QWidget *parent = NULL);
    ~EvaluationWidget();
    
    static std::string delim;

    public slots:
        void accept();
        void accepted();
private:
    Condition condition;
    int conditionIndex;
    QTime start;

    FILE * outFile;
    std::string filename;
    MyQGLWidget *myQGLWidget;

    QFormLayout *layout;
    QStackedWidget *stackedWidget;
    QDialogButtonBox *buttonBox;
    QLabel *warningMessage;
    
    void makeIntroPages();
    void makeTrainingPages();
    void makeExperimentPages();
    
    void makePage(const QString & message);

    void makeQuestion(const QString & question);
    void makeInstruction(const QString & message);

    void setCondition(std::string input);
    void setToCondition();
    void writeCondition();

    void attemptToAdvance();
    void makeWarning();
    QWidget *getCurrentWidget();
    bool isAdvancable();
    void advancePage();
    float getSecondsFromStart();

    template <typename T> std::string toStr(const T& t);

    void determineFilename(QString initials);

    void openFile();
    void closeFile();
    void writeToFile(std::string line);
};

#endif


