#include "InstructionalWidget.h"

#include <QGridLayout>
#include <QLabel>

InstructionalWidget::InstructionalWidget(const QString & message, QWidget *parent) : 
QWidget(parent) {
    QLabel * label = new QLabel(message);
    label->setStyleSheet("QLabel { color : blue;}");
    label->setAlignment(Qt::AlignCenter);
    
    QFont f( "Arial", 12, QFont::Bold);
    label->setFont(f);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(label, 0, 0);

    setLayout(layout);
}

InstructionalWidget::~InstructionalWidget() {

}
