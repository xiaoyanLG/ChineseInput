#ifndef XYPUSHBUTTON_H
#define XYPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>

class XYPushButton : public QWidget
{
    Q_OBJECT
public:
    explicit XYPushButton(const QString &text, QWidget *parent=0);
    ~XYPushButton();

signals:
    void clicked(bool checked = false);

private:
    QPushButton *btn;
};

#endif // XYPUSHBUTTON_H
