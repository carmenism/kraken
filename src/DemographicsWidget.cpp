#include "DemographicsWidget.h"

#include <QLineEdit>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QButtonGroup>
#include <QRadioButton>

DemographicsWidget::DemographicsWidget(QWidget *parent) : 
QWidget(parent) {
    initialsLabel = new QLabel("Enter initials:");
    
    initialsLineEdit = new QLineEdit(this);
    initialsLineEdit->show();

    genderLabel = new QLabel("Select gender:");

    genderComboBox = new QComboBox;
    genderComboBox->addItem(tr(""));
    genderComboBox->addItem(tr("Male"));
    genderComboBox->addItem(tr("Female"));

    schoolLabel = new QLabel("Select school:");

    schoolComboBox = new QComboBox;
    schoolComboBox->addItem(tr(""));
    schoolComboBox->addItem(tr("CEPS"));
    schoolComboBox->addItem(tr("CHHS"));
    schoolComboBox->addItem(tr("COLA"));
    schoolComboBox->addItem(tr("COLSA"));
    schoolComboBox->addItem(tr("Marine Sci & Ocean Eng"));
    schoolComboBox->addItem(tr("Paul College"));
    schoolComboBox->addItem(tr("TSAS"));
    
    layout = new QFormLayout;

    layout->addRow(initialsLabel, initialsLineEdit);
    layout->addRow(genderLabel, genderComboBox);
    layout->addRow(schoolLabel, schoolComboBox);

    setLayout(layout);
}

DemographicsWidget::~DemographicsWidget() {

}

QString DemographicsWidget::initials() {
    return initialsLineEdit->text();
}

QString DemographicsWidget::gender() {
    return genderComboBox->currentText();
}

QString DemographicsWidget::school() {
    return schoolComboBox->currentText();
}

bool DemographicsWidget::completed() {
    return !initials().isEmpty() && !gender().isEmpty() && !school().isEmpty();
}