#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xyinput.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit->installEventFilter(XYInput::getInstance());
    ui->lineEdit->installEventFilter(XYInput::getInstance());
}

MainWindow::~MainWindow()
{
    delete ui;
}
