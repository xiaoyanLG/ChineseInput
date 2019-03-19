#include "xysylixosxyinputmethod.h"
#include "xyinput.h"
#include <QInputMethodEvent>
#include <QScreen>
#include <QWindow>
#include <QFileInfo>
#include <QApplication>
#include <QMenu>
#include <QFileDialog>
#include <QDebug>

XYSylixOSXYInputMethod::XYSylixOSXYInputMethod(QObject *parent)
    : XYInputMethodInterface(parent)
{
    m_Parent = nullptr;
}

void XYSylixOSXYInputMethod::init(const QString &dllpath)
{
    initFirst();
    QFileInfo file(dllpath);
    if (!m_Input->initInputBase(
                file.absoluteDir().absolutePath() + "/chineseBase/chinese.db"))
    {
        qDebug() << "XYSylixOSXYInputMethod: Failed to load db!";
    }
}

QImage XYSylixOSXYInputMethod::icon()
{
    return QImage(":/xyinput.png");
}

QString XYSylixOSXYInputMethod::describe()
{
    return QStringLiteral("基于XY的中文输入法！");
}

void XYSylixOSXYInputMethod::show(QWindow *parent, const QPoint &cursorPos)
{
    m_Parent = parent;
    m_Pos = cursorPos;
}

void XYSylixOSXYInputMethod::hide()
{
    initFirst();
    m_Input->close();
}

bool XYSylixOSXYInputMethod::isVisible()
{
    initFirst();
    return m_Input->isVisible();
}

QRect XYSylixOSXYInputMethod::keyboardRectangle()
{
    initFirst();
    return m_Input->rect();
}

QLocale XYSylixOSXYInputMethod::locale()
{
    return QLocale();
}

Qt::LayoutDirection XYSylixOSXYInputMethod::inputDirection()
{
    return Qt::LeftToRight;
}

void XYSylixOSXYInputMethod::initFirst()
{
    if (m_Input == nullptr) {
        m_Input = XYInput::getInstance();

        connect(m_Input, &XYInput::send_preedit,
                this, &XYSylixOSXYInputMethod::send_preedit, Qt::DirectConnection);
        connect(m_Input, &XYInput::send_commit,
                this, &XYSylixOSXYInputMethod::send_commit, Qt::DirectConnection);
        connect(m_Input, &XYInput::send_keyEvent,
                this, &XYSylixOSXYInputMethod::send_keyEvent, Qt::DirectConnection);
    }
}

QMenu *XYSylixOSXYInputMethod::iconHandleMenu()
{
    static QMenu *menu = nullptr;
    if (menu == nullptr) {
        menu = new QMenu;
        QActionGroup *group = new QActionGroup(menu);
        QAction *act = menu->addAction(QStringLiteral("中文"), this, [](){
            XYInput::getInstance()->setEnglish(false);
        });
        act->setCheckable(true);
        act->setChecked(true);
        group->addAction(act);
        act = menu->addAction(QStringLiteral("英文"), this, [](){
            XYInput::getInstance()->setEnglish(true);
        });
        act->setCheckable(true);
        act->setChecked(false);
        group->addAction(act);
    }
    return menu;
}

void XYSylixOSXYInputMethod::handleKeyEvent(QKeyEvent *event)
{
    initFirst();
    if (m_Parent != nullptr) {
        m_Input->setScreen(m_Parent->screen());
    }

    m_Input->keyEvent(event);
}

void XYSylixOSXYInputMethod::send_keyEvent(QKeyEvent *event)
{
    emit sendKeyEvent(event);
}

void XYSylixOSXYInputMethod::send_preedit(const QString &text)
{
    QInputMethodEvent event(text, QList<QInputMethodEvent::Attribute>());
    emit sendInputMethod(&event);
}

void XYSylixOSXYInputMethod::send_commit(const QString &text)
{
    QInputMethodEvent event;
    event.setCommitString(text);
    emit sendInputMethod(&event);
}

