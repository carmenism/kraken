#ifndef _QUESTIONWIDGET_H
#define _QUESTIONWIDGET_H

#include <QtGui/QWidget>
#include <QString>

class QTextEdit;
class QLabel;
class QDialogButtonBox;
class QFormLayout;

class QuestionWidget : public QWidget {
    Q_OBJECT // must include this if you use Qt signals/slots

public:
    QuestionWidget(const QString & question, QWidget *parent = NULL);
    ~QuestionWidget();
    
    QString whatQuestion();
    QString whyQuestion();

    QString whatAnswer();
    QString whyAnswer();
private:
    QFormLayout *layout;

    //QDialogButtonBox *buttonBox;

    QLabel *whatLabel;
    QLabel *whyLabel;

    QTextEdit *whatTextEdit;
    QTextEdit *whyTextEdit;
};

#endif