#include "EvaluationWidget.h"

#include <QTextEdit>
#include <QFormLayout>
#include <QStackedWidget>
#include <QDialogButtonBox>
#include <QLabel>

#include "DemographicsWidget.h"
#include "QuestionWidget.h"
#include "MyQGLWidget.h"

#include <iostream>

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

    makeQuestion("What is the effect on herring?");
    makeQuestion("What is the effect on mackerel?");
    
    makeQuestion("What is the effect on yellowtail flounder?");
    makeQuestion("What is the effect on winter flounder?");
    
    makeQuestion("What is the effect on skates?");
    makeQuestion("What is the effect on spiny dogfish?");
    makeQuestion("What is the effect on windowpane?");
    makeQuestion("What is the effect on cod?");

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


void EvaluationWidget::makeQuestion(const QString & question) {
    QuestionWidget *page = new QuestionWidget(question);

    stackedWidget->addWidget(page);
}

void EvaluationWidget::accept() {
    std::cout << "evaluation accept\n";
    int currentIndex = stackedWidget->currentIndex();
    QWidget *currentWidget = stackedWidget->widget(currentIndex);

    if (currentIndex + 1 < stackedWidget->count()) {
        QuestionWidget *questionWidget = dynamic_cast<QuestionWidget *> (currentWidget);
        DemographicsWidget *demographicsWidget = dynamic_cast<DemographicsWidget *> (currentWidget);
        
        if (demographicsWidget) {
            if (demographicsWidget->completed()) {
                warningMessage->setText("");
                stackedWidget->setCurrentIndex(currentIndex + 1);
            } else {
                warningMessage->setText("Please fill out all fields.");
            }
        } else if (questionWidget) {            
            if (questionWidget->completed()) {
                // advance 
                warningMessage->setText("");
                stackedWidget->setCurrentIndex(currentIndex + 1);
            } else {
                warningMessage->setText("Please fill out all fields.");
            }
        }        
    } else {
        // end evaluation
    }
}


void EvaluationWidget::accepted() {
    std::cout << "evaluation accepted\n";
}