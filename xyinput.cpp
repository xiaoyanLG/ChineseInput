#include "xyinput.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>

XYInput *XYInput::mopInstance;
XYInput *XYInput::getInstance()
{
    if (mopInstance == NULL)
    {
        mopInstance = new XYInput;
    }
    return mopInstance;
}

XYInput::XYInput(QWidget *parent)
    : XYMovableWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                         | Qt::WindowStaysOnTopHint
                         | Qt::WindowType_Mask);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(QMargins(15, 15, 20, 15));
    mopLineEdit = new QLineEdit;
    mopLineEdit->installEventFilter(this);
    layout->addWidget(mopLineEdit);

    setLayout(layout);
}

XYInput::~XYInput()
{

}

bool XYInput::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && obj->isWidgetType())
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (obj == mopLineEdit)
        {
            switch (keyEvent->key())
            {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                qApp->postEvent(mopLatestWidget, new QKeyEvent(QEvent::KeyPress, Qt::Key_unknown, Qt::NoModifier, mopLineEdit->text()));
                close();
                break;
            default:
                break;
            }
        }
        else if (keyEvent->key() != Qt::Key_unknown && keyEvent->modifiers() == Qt::NoModifier)
        {
            mopLatestWidget = static_cast<QWidget *>(obj);
            qDebug("Ate key press %d", keyEvent->key());
            XYInput *input = XYInput::getInstance();
            input->mopLineEdit->clear();
            input->move(QCursor::pos());
            input->show();

            qApp->postEvent(input->mopLineEdit, new QKeyEvent(QEvent::KeyPress, keyEvent->key(), Qt::NoModifier, keyEvent->text()));
            return true;
        }
    }
    else if (QEvent::InputMethod == event->type())
    {
        QInputMethodEvent *me = static_cast<QInputMethodEvent *>(event);
//        qDebug() << me->attributes().size();
        for (int i = 0; i < me->attributes().size(); ++i)
        {
            qDebug() << me->attributes().at(i).type;
        }
        qDebug("Text: %s", me->commitString().toUtf8().data());
    }
    return QWidget::eventFilter(obj, event);
}

