#include "QuestionWidget.h"

#include <QTextEdit>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QComboBox>

QuestionWidget::QuestionWidget(const QString & question, QWidget *parent) : 
ResponseWidget(parent) {
    whatLabel = new QLabel(question);
    
    //whatTextEdit = new QLineEdit(this);
    //whatTextEdit->setText("ENTER ANSWER HERE");
    //whatTextEdit->show();

    whatComboBox = new QComboBox;
    whatComboBox->addItem(tr(""));
    whatComboBox->addItem(tr("Increased a lot"));
    whatComboBox->addItem(tr("Increased a little"));
    whatComboBox->addItem(tr("Stayed about the same"));
    whatComboBox->addItem(tr("Decreased a little"));
    whatComboBox->addItem(tr("Decreased a lot"));

    whyLabel = new QLabel("Why? [Try to explain in no more than three sentences.]");

    whyTextEdit = new QTextEdit(this);
    //whyTextEdit->setText("ENTER ANSWER HERE");
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
    return "Why?";//whyLabel->text();
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
    return whatQuestion().toStdString() + "\t"
        + whatAnswer().toStdString() + "\t"
        + whyQuestion().toStdString() + "\t"
        + whyAnswer().toStdString();
}