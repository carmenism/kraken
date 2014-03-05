#include "EvaluationWidget.h"

#include <QTextEdit>
#include <QFormLayout>
#include <QStackedWidget>
#include <QDialogButtonBox>

#include "QuestionWidget.h"
#include "MyQGLWidget.h"

#include <iostream>

EvaluationWidget::EvaluationWidget(MyQGLWidget *myQGLWidget, QWidget *parent) : 
QWidget(parent) {
    this->myQGLWidget = myQGLWidget;
    //resize(320, 240);
    show();
    setWindowTitle("Evaluation");

     /*QWidget *firstPageWidget = new QWidget;
     QWidget *secondPageWidget = new QWidget;
     QWidget *thirdPageWidget = new QWidget;

     QStackedWidget *stackedWidget = new QStackedWidget;
     addWidget(firstPageWidget);
     addWidget(secondPageWidget);
     addWidget(thirdPageWidget);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(stackedWidget);
     setLayout(layout);*/
    stackedWidget = new QStackedWidget(this);
    stackedWidget->show();

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
        QuestionWidget *questionWidget = static_cast<QuestionWidget *> (currentWidget);
        if (questionWidget) {
            QString what = questionWidget->whatAnswer();
            QString why = questionWidget->whyAnswer();
            
            if (what.isEmpty() || why.isEmpty()) {

            } else {
                // advance 
                stackedWidget->setCurrentIndex(currentIndex + 1);
            }
        }        
    } else {
        // end evaluation
    }
}


void EvaluationWidget::accepted() {
    std::cout << "evaluation accepted\n";
}