#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void send_preedit(const QString &str);
    void send_commit(const QString &str);
    void send_keyEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
