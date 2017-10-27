#ifndef XYPUSHBUTTON_H
#define XYPUSHBUTTON_H

#include <QLabel>

class XYPushButton : public QLabel
{
    Q_OBJECT
public:
    explicit XYPushButton(const QString &text, int generalKey, int shiftKey = -1, bool small = false, QWidget *parent=0);
    ~XYPushButton();
    bool isChecked();

signals:
    void clicked();
    void clicked(int key);
    void checkedChanged(bool checked = false);

public slots:
    void setChecked(bool checked);
    void setCheckable(bool checkable);

protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool checkable;
    bool checked;
    bool pressed;
    bool isSmallAreaKey;   // 记录是否是小键盘区域的按键

    int shiftKey;
    int generalKey;

    static bool capsLocked;
    static bool numLocked;
    static Qt::KeyboardModifiers modifiers;
};

#endif // XYPUSHBUTTON_H
