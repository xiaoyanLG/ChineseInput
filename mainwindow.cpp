#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xyinput.h"
#include "xyvirtualkeyboard.h"
#include <QApplication>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDir::setCurrent(qApp->applicationDirPath());
//    XYInput::getInstance()->initInputBase("I:/QtProject/ChineseInput/chineseBase/chinese.db");
    XYInput::getInstance()->initInputBase("chineseBase/chinese.db");
    ui->textEdit->installEventFilter(XYInput::getInstance());
    ui->lineEdit->installEventFilter(XYInput::getInstance());

    XYVirtualKeyboard *bo = XYVirtualKeyboard::getInstance();
    bo->showSmall();
}

MainWindow::~MainWindow()
{
    delete ui;
}
