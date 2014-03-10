#ifndef _DEMOGRAPHICSWIDGET_H
#define _DEMOGRAPHICSWIDGET_H

#include <QtGui/QWidget>
#include <QString>
#include "ResponseWidget.h"

class QLineEdit;
class QLabel;
class QFormLayout;
class QComboBox;

class DemographicsWidget : public ResponseWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    DemographicsWidget(QWidget *parent = NULL);
    ~DemographicsWidget();
    
    QString initials();
    QString gender();
    QString school();

    virtual bool completed();
    virtual std::string getLine();
private:
    QFormLayout *layout;

    QLabel *initialsLabel;
    QLabel *genderLabel;
    QLabel *schoolLabel;

    QLineEdit *initialsLineEdit;
    QComboBox *genderComboBox;
    QComboBox *schoolComboBox;
};

#endif