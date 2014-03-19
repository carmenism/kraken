#include "ConditionSelectorWidget.h"

#include "EvaluationWidget.h"
#include <QLabel>
#include <QFormLayout>
#include <QComboBox>

ConditionSelectorWidget::ConditionSelectorWidget(QWidget *parent)
: ResponseWidget(parent) {
    conditionLabel = new QLabel("Select experiment condition:");
    
    conditionComboBox = new QComboBox;
    conditionComboBox->addItem(tr(""));
    conditionComboBox->addItem(tr("Condition A"));
    conditionComboBox->addItem(tr("Condition B"));
    conditionComboBox->addItem(tr("Condition C"));
    conditionComboBox->addItem(tr("Condition D"));
    
    layout = new QFormLayout;

    layout->addRow(conditionLabel, conditionComboBox);

    setLayout(layout);
    
    setFocusProxy(conditionComboBox);
}

ConditionSelectorWidget::~ConditionSelectorWidget() {
    delete conditionLabel;
    delete conditionComboBox;
    delete layout;
}

QString ConditionSelectorWidget::condition() {
    return conditionComboBox->currentText();
}

bool ConditionSelectorWidget::completed() {
    return !condition().isEmpty();
}

std::string ConditionSelectorWidget::getLine() {
   return "Condition" + EvaluationWidget::delim 
       + condition().toStdString();
}