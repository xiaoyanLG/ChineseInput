#ifndef XYVIRTUALKEYBOARD_H
#define XYVIRTUALKEYBOARD_H

#include <QWidget>

class XYVirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    explicit XYVirtualKeyboard(QWidget *parent = 0);
    ~XYVirtualKeyboard();

public slots:

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    QWidget *curWidget;

};

#endif // XYVIRTUALKEYBOARD_H
