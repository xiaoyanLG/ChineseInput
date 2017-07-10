#include "xyinput.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QAbstractItemView>
#include <QListView>
#include <Windows.h>
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
    : XYBorderShadowWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool
                   | Qt::WindowDoesNotAcceptFocus);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(QMargins(13, 13, 20, 13));
    mopLineEdit = new QLineEdit;
    mopLineEdit->installEventFilter(this);
    layout->addWidget(mopLineEdit);
    connect(mopLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(mslotFindTranslate(const QString &)));

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
            XYInput *input = XYInput::getInstance();
            if (!input->isVisible())
            {
                input->mopLineEdit->clear();
                input->move(QCursor::pos());
                input->show();
            }

            qApp->postEvent(input->mopLineEdit, new QKeyEvent(QEvent::KeyPress, keyEvent->key(), Qt::NoModifier, keyEvent->text()));
            return true;
        }
    }
    else if (QEvent::InputMethod == event->type())
    {
        // 这里是输入法传入的信号，不过，我们可以不用
        QInputMethodEvent *me = static_cast<QInputMethodEvent *>(event);
        if (me->attributes().size() == 1 && me->attributes().at(0).type == 1)
        {
            qDebug("Text: %s", me->commitString().toUtf8().data());
        }
        // 如果想屏蔽，直接返回true
//        return true;
    }
    else if (QEvent::FocusOut == event->type())
    {
        XYInput *input = XYInput::getInstance();
        input->close();
    }
    return QWidget::eventFilter(obj, event);
}

void XYInput::mslotFindTranslate(const QString &keyword)
{
    load();
}

void XYInput::load()
{

}

