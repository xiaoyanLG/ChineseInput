#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xyinput.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDir::setCurrent(qApp->applicationDirPath());
    if (!XYInput::getInstance()->initInputBase("chineseBase/chinese.db"))
    {
        XYInput::getInstance()->initInputBase("../../ChineseInput/chineseBase/chinese.db");
    }
    ui->textEdit->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);

    connect(XYInput::getInstance(), &XYInput::send_preedit, this, [](const QString &text){
        if (qApp->focusWidget()) {
            QInputMethodEvent event(text, QList<QInputMethodEvent::Attribute>());
            qApp->sendEvent(qApp->focusWidget(), &event);
        }
    });
    connect(XYInput::getInstance(), &XYInput::send_commit, this, [](const QString &text){
        if (qApp->focusWidget()) {
            QInputMethodEvent event;
            event.setCommitString(text);
            qApp->sendEvent(qApp->focusWidget(), &event);
        }
    });
    connect(XYInput::getInstance(), &XYInput::send_keyEvent, this, [](QKeyEvent *keyevent){
        if (qApp->focusWidget()) {
            qApp->postEvent(qApp->focusWidget(), new QKeyEvent(*keyevent));
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->spontaneous() &&
            (event->type() == QEvent::KeyPress
            || event->type() == QEvent::KeyRelease))
    {
        XYInput::getInstance()->keyEvent(static_cast<QKeyEvent *>(event));
        return true;
    }

    return QMainWindow::eventFilter(obj, event);
}
