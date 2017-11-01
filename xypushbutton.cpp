#include "xypushbutton.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>

bool XYPushButton::capsLocked = false;
bool XYPushButton::numLocked = false;
Qt::KeyboardModifiers XYPushButton::modifiers = 0;
XYPushButton::XYPushButton(const QString &text, int generalKey, int shiftKey, bool small, QWidget *parent)
    : QLabel(text, parent),
      generalKey(generalKey),
      shiftKey(shiftKey),
      isSmallAreaKey(small),
      checked(false),
      pressed(false),
      checkable(false)
{
//    QFont font = this->font();
//    font.setBold(true);
//    setFont(font);
    setMinimumWidth(45);
    setMinimumHeight(30);
    setAlignment(Qt::AlignCenter);
}

XYPushButton::~XYPushButton()
{

}

bool XYPushButton::isChecked()
{
    return checked;
}

void XYPushButton::setCheckable(bool checkable)
{
    this->checkable = checkable;
}

void XYPushButton::setChecked(bool checked)
{
    this->checked = checked;
    update();
}

void XYPushButton::resizeEvent(QResizeEvent *event)
{
//    QFont font = this->font();
//    QFontMetrics metrics(font);
//    if (metrics.width(text()) > event->size().width())
//    {
//        while (metrics.width(text()) > event->size().width()
//               && metrics.height() > event->size().height())
//        {
//            font.setPointSize(font.pointSize() - 1);
//            metrics = QFontMetrics(font);
//        }
//        font.setPointSize(font.pointSize() + 1);
//    }
//    else
//    {
//        while (metrics.width(text()) < event->size().width()
//               && metrics.height() < event->size().height())
//        {
//            font.setPointSize(font.pointSize() + 1);
//            metrics = QFontMetrics(font);
//        }
//        font.setPointSize(font.pointSize() - 1);
//    }
//    this->setFont(font);
    QLabel::resizeEvent(event);
}

void XYPushButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawRect(rect().x(), rect().y(), rect().width() - 1, rect().height() - 1);
    if (generalKey == Qt::Key_Menu)
    {
        QBrush brush = painter.brush();
        painter.setBrush(QColor(0xb2, 0x50, 0x50));
        painter.drawRect(rect().x(), rect().y(), rect().width() - 1, rect().height() - 1);
        painter.setBrush(brush);
    }
    if (pressed || checked)
    {
        QBrush brush = painter.brush();
        painter.setBrush(QColor(77, 0xff, 0xff, 0xff));
        painter.drawRect(rect().x(), rect().y(), rect().width() - 1, rect().height() - 1);
        painter.setBrush(brush);
    }
    QLabel::paintEvent(event);
}

void XYPushButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        pressed = true;
        update();
    }
    emit clicked();
    event->accept();
}

void XYPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (rect().contains(event->pos()))
        {
            if (checkable)
            {
                checked = !checked;
                emit checkedChanged(checked);
            }
            bool general_key = false;
            if (checked)
            {
                switch (generalKey)
                {
                case Qt::Key_Shift:
                    modifiers |= Qt::ShiftModifier;
                    break;
                case Qt::Key_Meta:
                    modifiers |= Qt::MetaModifier;
                    break;
                case Qt::Key_Control:
                    modifiers |= Qt::ControlModifier;
                    break;
                case Qt::Key_Alt:
                    modifiers |= Qt::AltModifier;
                    break;
                case Qt::Key_CapsLock:
                    capsLocked = true;
                    break;
                case Qt::Key_NumLock:
                    numLocked = true;
                    break;
                case Qt::Key_Menu:
                    break;
                default:
                    general_key = true;
                    break;
                }
            }
            else
            {
                switch (generalKey)
                {
                case Qt::Key_Shift:
                    modifiers &= !Qt::ShiftModifier;
                    break;
                case Qt::Key_Meta:
                    modifiers &= !Qt::MetaModifier;
                    break;
                case Qt::Key_Control:
                    modifiers &= !Qt::ControlModifier;
                    break;
                case Qt::Key_Alt:
                    modifiers &= !Qt::AltModifier;
                    break;
                case Qt::Key_CapsLock:
                    capsLocked = false;
                    break;
                case Qt::Key_NumLock:
                    numLocked = false;
                    break;
                case Qt::Key_Menu:
                    break;
                default:
                    general_key = true;
                    break;
                }
            }

            int key = generalKey;
            QString text = QChar(key);
            if (general_key)
            {
                if (shiftKey != -1)
                {
                    if (modifiers & Qt::ShiftModifier)
                    {
                        key = shiftKey;
                        text = QChar(key);
                    }
                }
                else if (Qt::Key_A <= generalKey && generalKey <= Qt::Key_Z)
                {
                    if (capsLocked || (modifiers & Qt::ShiftModifier))
                    {
                        text = QChar(generalKey);
                    }
                    else
                    {
                        text = QChar(generalKey + 32);
                    }
                }

                if (!numLocked || !isSmallAreaKey)
                {
                    if (qApp->focusWidget() != NULL)
                    {
                        qApp->postEvent(qApp->focusWidget(),
                                        new QKeyEvent(QKeyEvent::KeyPress, key, modifiers, text));
                        qApp->postEvent(qApp->focusWidget(),
                                        new QKeyEvent(QKeyEvent::KeyRelease, key, modifiers, text));
                    }
                }
            }
        }
        pressed = false;
        update();
    }
    event->accept();
}

