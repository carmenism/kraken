#ifndef _CONDITIONSELECTORSWIDGET_H
#define _CONDITIONSELECTORSWIDGET_H

#include <QtGui/QWidget>
#include <QString>
#include "ResponseWidget.h"

class QLabel;
class QFormLayout;
class QComboBox;

class ConditionSelectorWidget : public ResponseWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    ConditionSelectorWidget(QWidget *parent = NULL);
    ~ConditionSelectorWidget();
    
    QString condition();

    virtual bool completed();
    virtual std::string getLine();
private:
    QFormLayout *layout;

    QLabel *conditionLabel;

    QComboBox *conditionComboBox;
};

#endif