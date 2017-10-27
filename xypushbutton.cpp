#include "xypushbutton.h"
#include <QHBoxLayout>

XYPushButton::XYPushButton(const QString &text, QWidget *parent)
    : QWidget(parent)
{
    btn = new QPushButton(text);
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(btn);
    connect(btn, SIGNAL(clicked(bool)), this, SIGNAL(clicked(bool)));
}

XYPushButton::~XYPushButton()
{

}

