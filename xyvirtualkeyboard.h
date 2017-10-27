#ifndef XYVIRTUALKEYBOARD_H
#define XYVIRTUALKEYBOARD_H

#include <QWidget>

class XYPushButton;
class XYVirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    static XYVirtualKeyboard *getInstance();
    ~XYVirtualKeyboard();

public slots:
    void enlarge();
    void shrink();
    void show();
    void showMain();
    void showSmall();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

protected:
    bool          mbLeftMousePressed;
    QPoint        moLastPos;

private:
    static XYVirtualKeyboard *instance;
    explicit XYVirtualKeyboard(QWidget *parent = 0);
    QWidget *curWidget;

    XYPushButton *smallNumLockBtn;
    QWidget *mainWidget;
    QWidget *smallWidget;

};

#endif // XYVIRTUALKEYBOARD_H
