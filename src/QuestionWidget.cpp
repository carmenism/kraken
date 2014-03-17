#include "QuestionWidget.h"

#include <QTextEdit>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QLineEdit>

QuestionWidget::QuestionWidget(const QString & question, QWidget *parent) : 
ResponseWidget(parent) {
    whatLabel = new QLabel(question);
    
    whatTextEdit = new QLineEdit(this);
    //whatTextEdit->setText("ENTER ANSWER HERE");
    whatTextEdit->show();

    whyLabel = new QLabel("Why? [Try to explain in no more than three sentences.]");

    whyTextEdit = new QTextEdit(this);
    //whyTextEdit->setText("ENTER ANSWER HERE");
    whyTextEdit->show();
    whyTextEdit->setTabChangesFocus(true);
    
    layout = new QFormLayout;

    layout->addRow(whatLabel);
    layout->addRow(whatTextEdit);
    layout->addRow(whyLabel);
    layout->addRow(whyTextEdit);

    setLayout(layout);
}

QuestionWidget::~QuestionWidget() {
    delete whatLabel;
    delete whyLabel;
    delete whatTextEdit;
    delete whyTextEdit;
    delete layout;
}

QString QuestionWidget::whatQuestion() {
    return whatLabel->text();
}

QString QuestionWidget::whyQuestion() {
    return "Why?";whyLabel->text();
}

QString QuestionWidget::whatAnswer() {
    return whatTextEdit->text();
}

QString QuestionWidget::whyAnswer() {
    return whyTextEdit->toPlainText();
}

bool QuestionWidget::completed() {
    return !whatAnswer().isEmpty() && !whyAnswer().isEmpty();
}

std::string QuestionWidget::getLine() {
    return whatQuestion().toStdString() + "\t"
        + whatAnswer().toStdString() + "\t"
        + whyQuestion().toStdString() + "\t"
        + whyAnswer().toStdString();
}