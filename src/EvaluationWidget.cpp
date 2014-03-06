#include "EvaluationWidget.h"

#include <QTextEdit>
#include <QFormLayout>
#include <QStackedWidget>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDateTime>
#include <QTime>
#include <QTime>

#include "DemographicsWidget.h"
#include "QuestionWidget.h"
#include "ResponseWidget.h"
#include "InstructionalWidget.h"
#include "MyQGLWidget.h"

#include <iostream>
#include <sstream>

EvaluationWidget::EvaluationWidget(MyQGLWidget *myQGLWidget, QWidget *parent) : 
QWidget(parent) {
    this->myQGLWidget = myQGLWidget;
    resize(600, 400);
    show();
    setWindowTitle("Evaluation");

    stackedWidget = new QStackedWidget(this);
    stackedWidget->show();

    stackedWidget->addWidget(new DemographicsWidget());

    warningMessage = new QLabel("");
    warningMessage->setStyleSheet("QLabel { color : red; }");

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    makeInstruction("Click OK to start experiment.");

    makeInstruction("Using the sliders, double the fishing effort of small pelagics.");
    makeQuestion("What is the effect on herring?");
    makeQuestion("What is the effect on mackerel?");
        
    makeInstruction("Using the sliders, halve the fishing effort of flatfish.");
    makeQuestion("What is the effect on yellowtail flounder?");
    makeQuestion("What is the effect on winter flounder?");
        
    makeInstruction("Using the sliders, double the fishing effort of elasmobranchs.");
    makeQuestion("What is the effect on skates?");
    makeQuestion("What is the effect on spiny dogfish?");
    makeQuestion("What is the effect on windowpane?");
    makeQuestion("What is the effect on cod?");

    makeInstruction("End of experiment.  Thank you for participating.");
    
    layout = new QFormLayout;
    layout->addRow(stackedWidget);
    layout->addRow(warningMessage);
    layout->addRow(buttonBox);

    setLayout(layout);
}

EvaluationWidget::~EvaluationWidget() {
    delete stackedWidget;
    delete layout;
}

void EvaluationWidget::makeInstruction(const QString & message) {
    InstructionalWidget *page = new InstructionalWidget(message);

    stackedWidget->addWidget(page);
}

void EvaluationWidget::makeQuestion(const QString & question) {
    QuestionWidget *page = new QuestionWidget(question);

    stackedWidget->addWidget(page);
}

void EvaluationWidget::accept() {    
    if (isAdvancable()) {
        attemptToAdvance();
    } else {
        // end evaluation
        closeFile();
        exit(0);
    }
}

void EvaluationWidget::attemptToAdvance() {    
    QWidget *currentWidget = getCurrentWidget();
    ResponseWidget *responseWidget = dynamic_cast<ResponseWidget *> (currentWidget);
    InstructionalWidget *instructionWidget = dynamic_cast<InstructionalWidget *> (currentWidget);

    if (responseWidget) {
        QuestionWidget *quest = dynamic_cast<QuestionWidget *> (currentWidget);
        DemographicsWidget *demo = dynamic_cast<DemographicsWidget *> (currentWidget);

        if (responseWidget->completed()) {            
            std::string line = responseWidget->getLine();

            if (demo) {
                determineFilename(demo->initials());
                openFile();
            } else if (quest) {
                std::cout << "End question\n";
                line = line + "\t" + toStr(getSecondsFromStart());
            }

            writeToFile(line);
            advancePage();
        } else {
            makeWarning();
        }
    } else if (instructionWidget) {
        myQGLWidget->resetAllSliders();
        advancePage();
    }
}

float EvaluationWidget::getSecondsFromStart() {
    QTime end = QDateTime::currentDateTime().time();
    int msec = start.msecsTo(end);
    float sec = msec / 1000.0;

    return sec;
}

template <typename T> std::string EvaluationWidget::toStr(const T& t) { 
    std::ostringstream os;
    os << t; 
    return os.str();
}

void EvaluationWidget::makeWarning() {
    warningMessage->setText("Please fill out all fields.");
}

QWidget *EvaluationWidget::getCurrentWidget() {
    int currentIndex = stackedWidget->currentIndex();
    
    return stackedWidget->widget(currentIndex);
}

bool EvaluationWidget::isAdvancable() {
    int currentIndex = stackedWidget->currentIndex();

    return currentIndex + 1 < stackedWidget->count();
}

void EvaluationWidget::advancePage() {
    int currentIndex = stackedWidget->currentIndex();
    warningMessage->setText("");
    stackedWidget->setCurrentIndex(currentIndex + 1);

    QWidget *currentWidget = getCurrentWidget();
    QuestionWidget *quest = dynamic_cast<QuestionWidget *> (currentWidget);
    
    if (quest) {
        std::cout << "Start question\n";
        start = QDateTime::currentDateTime().time();
    }
}

void EvaluationWidget::writeToFile(std::string line) {
    std::cout << line + "\n";
    fputs(line.c_str(), outFile);
    fputs("\n", outFile);
}

void EvaluationWidget::openFile() {
    outFile = fopen(filename.c_str(), "w");
}

void EvaluationWidget::closeFile() {
    fclose(outFile);
}

void EvaluationWidget::determineFilename(QString initials) {
    QDateTime now = QDateTime::currentDateTime();
    QString date = now.date().toString("yyyy.MM.dd");
    QString time = now.time().toString("HH.mm");
    filename = date.toStdString() + "_" + time.toStdString() + "_" + initials.toStdString() + ".csv";
}

void EvaluationWidget::accepted() {
    std::cout << "evaluation accepted\n";
}