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

    connect(XYInput::getInstance(), SIGNAL(send_preedit(QString)),
            this, SLOT(send_preedit(QString)));
    connect(XYInput::getInstance(), SIGNAL(send_commit(QString)),
            this, SLOT(send_commit(QString)));
    connect(XYInput::getInstance(), SIGNAL(send_keyEvent(QKeyEvent *)),
            this, SLOT(send_keyEvent(QKeyEvent *)));
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

void MainWindow::send_preedit(const QString &str)
{
    if (qApp->focusWidget()) {
        QInputMethodEvent event(str, QList<QInputMethodEvent::Attribute>());
        qApp->sendEvent(qApp->focusWidget(), &event);
    }
}

void MainWindow::send_commit(const QString &str)
{
    if (qApp->focusWidget()) {
        QInputMethodEvent event;
        event.setCommitString(str);
        qApp->sendEvent(qApp->focusWidget(), &event);
    }
}

void MainWindow::send_keyEvent(QKeyEvent *event)
{
    if (qApp->focusWidget()) {
        qApp->postEvent(qApp->focusWidget(), new QKeyEvent(*event));
    }
}
