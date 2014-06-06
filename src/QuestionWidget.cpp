#include "QuestionWidget.h"

#include "EvaluationWidget.h"
#include <QTextEdit>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QComboBox>

QuestionWidget::QuestionWidget(const QString & question, QWidget *parent) : 
ResponseWidget(parent) {
    whatLabel = new QLabel(question);
    
    whatComboBox = new QComboBox;
    whatComboBox->addItem(tr(""));
    whatComboBox->addItem(tr("Increased a lot"));
    whatComboBox->addItem(tr("Increased a little"));
    whatComboBox->addItem(tr("Stayed about the same"));
    whatComboBox->addItem(tr("Decreased a little"));
    whatComboBox->addItem(tr("Decreased a lot"));

    whyLabel = new QLabel("Why? [Try to explain in no more than three sentences.]");

    whyTextEdit = new QTextEdit(this);
    whyTextEdit->show();
    whyTextEdit->setTabChangesFocus(true);
    
    layout = new QFormLayout;

    layout->addRow(whatLabel);
    layout->addRow(whatComboBox);
    layout->addRow(whyLabel);
    layout->addRow(whyTextEdit);

    setLayout(layout);

    setFocusProxy(whatComboBox);
    setTabOrder(whatComboBox, whyTextEdit);
    
    QFont f( "Arial", EvaluationWidget::fontSize);
    whatLabel->setFont(f);
    whyLabel->setFont(f);
    whatComboBox->setFont(f);
    whyTextEdit->setFont(f);
}

QuestionWidget::~QuestionWidget() {
    delete whatLabel;
    delete whyLabel;
    delete whatComboBox;
    delete whyTextEdit;
    delete layout;
}

QString QuestionWidget::whatQuestion() {
    return whatLabel->text();
}

QString QuestionWidget::whyQuestion() {
    return "Why?";
}

QString QuestionWidget::whatAnswer() {
    return whatComboBox->currentText();
}

QString QuestionWidget::whyAnswer() {
    return whyTextEdit->toPlainText();
}

bool QuestionWidget::completed() {
    return !whatAnswer().isEmpty() && !whyAnswer().isEmpty();
}

std::string QuestionWidget::getLine() {
    QString newAnswer = QString(whyAnswer());

    int j = 0;

    while ((j = newAnswer.indexOf("\"", j)) != -1) {
        newAnswer.replace(j, 1, "\"\"");
        j = j + 2;
    }

    return whatQuestion().toStdString() + EvaluationWidget::delim
        + whatAnswer().toStdString() + EvaluationWidget::delim
        + whyQuestion().toStdString() + EvaluationWidget::delim
        + "\"" + newAnswer.toStdString() + "\"";
}