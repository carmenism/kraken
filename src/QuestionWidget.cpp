#include "QuestionWidget.h"

#include <QTextEdit>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>

#include <iostream>

QuestionWidget::QuestionWidget(const QString & question, QWidget *parent) : 
QWidget(parent) {
    whatLabel = new QLabel(question);
    
    whatTextEdit = new QTextEdit(this);
    //whatTextEdit->setText("ENTER ANSWER HERE");
    whatTextEdit->show();

    whyLabel = new QLabel("Why?");

    whyTextEdit = new QTextEdit(this);
    //whyTextEdit->setText("ENTER ANSWER HERE");
    whyTextEdit->show();
    
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
    return whyLabel->text();
}

QString QuestionWidget::whatAnswer() {
    return whatTextEdit->toPlainText();
}

QString QuestionWidget::whyAnswer() {
    return whyTextEdit->toPlainText();
}