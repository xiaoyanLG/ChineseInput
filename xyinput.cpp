#include "xyinput.h"
#include "xydatabaseoperation.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>
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
    layout->setContentsMargins(QMargins(13, 13, 13, 13));
    mopTransLateView = new XYTranslateView;
    mopLineEdit = new QLineEdit;
    mopLineEdit->installEventFilter(this);
    layout->addWidget(mopLineEdit);
    connect(mopLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(mslotFindTranslate(const QString &)));

    setLayout(layout);
}

bool XYInput::initInputBase(const QString &path)
{
    return XYDB->openDatabaseFile(path);
}

XYInput::~XYInput()
{
    delete mopTransLateView;
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
                qApp->postEvent(mopLatestWidget, new QKeyEvent(QEvent::KeyPress,
                                                               Qt::Key_unknown,
                                                               Qt::NoModifier,
                                                               mopLineEdit->text()));
                close();
                break;
            case Qt::Key_Equal:
                mopTransLateView->nextPage();
                mopTransLateView->update();
                return true;
            case Qt::Key_Minus:
                mopTransLateView->prePage();
                mopTransLateView->update();
                return true;
            case Qt::Key_Space:
                qApp->postEvent(mopLatestWidget, new QKeyEvent(QEvent::KeyPress,
                                                               Qt::Key_unknown,
                                                               Qt::NoModifier,
                                                               mopTransLateView->getData(1)));
                close();
                return true;
            default:
                break;
            }
            if (keyEvent->key() >= Qt::Key_0 && keyEvent->key() <= Qt::Key_9 && keyEvent->modifiers() != Qt::KeypadModifier)
            {
                int index = keyEvent->text().toInt();
                if (index > 0 && index <= mopTransLateView->miMaxVisibleItem && mopTransLateView->itemCount() >= index)
                {
                    qApp->postEvent(mopLatestWidget, new QKeyEvent(QEvent::KeyPress,
                                                                   Qt::Key_unknown,
                                                                   Qt::NoModifier,
                                                                   mopTransLateView->getData(index)));
                    close();
                }
                return true;
            }
        }
        else if (keyEvent->key() != Qt::Key_unknown &&
                 (keyEvent->modifiers() == Qt::NoModifier || keyEvent->modifiers() == Qt::KeypadModifier))
        {
            mopLatestWidget = static_cast<QWidget *>(obj);
            XYInput *input = XYInput::getInstance();
            if (!input->isVisible())
            {
                if (keyEvent->key() >= Qt::Key_A && keyEvent->key() <= Qt::Key_Z)
                {
                    input->mopLineEdit->clear();
                    input->mopTransLateView->clear();
                    input->show();
                    qApp->postEvent(input->mopLineEdit, new QKeyEvent(QEvent::KeyPress,
                                                                      keyEvent->key(),
                                                                      keyEvent->modifiers(),
                                                                      keyEvent->text()));
                    return true;
                }
            }
            else
            {
                qApp->postEvent(input->mopLineEdit, new QKeyEvent(QEvent::KeyPress,
                                                                  keyEvent->key(),
                                                                  keyEvent->modifiers(),
                                                                  keyEvent->text()));
                return true;
            }
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
        // return true;
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
    if (keyword.isEmpty()) // 如果传入的词为空了，代表删完了，应该关闭输入窗口
    {
        close();
        return;
    }
    XYDB->createInputTable();
    QList<XYTranslateItem *> list = XYDB->findData(keyword + "%", "basePintying");
    mopTransLateView->setData(list);
    load();
}

bool XYInput::close()
{
    mopTransLateView->clear();
    mopTransLateView->repaint(); // 清理view,避免显示的时候刷新
    mopTransLateView->close();
    return XYBorderShadowWidget::close();
}

void XYInput::show()
{
    QDesktopWidget *desk = qApp->desktop();
    int pos_x, pos_y;
    pos_x = QCursor::pos().x();
    pos_y = QCursor::pos().y();
    if (pos_x + this->width() > desk->width())
    {
        pos_x = desk->width() - this->width();
    }
    if (pos_y + this->height() > desk->height())
    {
        pos_y = desk->height() - this->height();
    }

    this->move(pos_x, pos_y);
    QWidget::show();
}

void XYInput::load()
{
    if (mopTransLateView->itemCount() == 0) // 当没有找到词组，直接关闭退出
    {
        mopTransLateView->close();
        return;
    }
    QDesktopWidget *desk = qApp->desktop();
    int pos_x, pos_y;
    pos_x = this->pos().x();
    pos_y = this->pos().y() + this->height() - 15;
    if (pos_x + mopTransLateView->width() > desk->width())
    {
        pos_x = desk->width() - mopTransLateView->width();
    }
    if (pos_y + mopTransLateView->height() > desk->height())
    {
        pos_y = this->pos().y() - mopTransLateView->height() + 15;
    }

    if (mopTransLateView->pos() != QPoint(pos_x, pos_y))
    {
        mopTransLateView->move(pos_x, pos_y);
    }
    mopTransLateView->show();
}

