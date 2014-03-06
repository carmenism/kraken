#ifndef _DEMOGRAPHICSWIDGET_H
#define _DEMOGRAPHICSWIDGET_H

#include <QtGui/QWidget>
#include <QString>

class QLineEdit;
class QLabel;
class QDialogButtonBox;
class QFormLayout;
class QComboBox;

class DemographicsWidget : public QWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    DemographicsWidget(QWidget *parent = NULL);
    ~DemographicsWidget();
    
    QString initials();
    QString gender();
    QString school();

    bool completed();
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