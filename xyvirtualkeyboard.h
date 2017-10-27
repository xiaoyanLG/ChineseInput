#ifndef XYVIRTUALKEYBOARD_H
#define XYVIRTUALKEYBOARD_H

#include <QWidget>

class XYVirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    static XYVirtualKeyboard *getInstance();
    ~XYVirtualKeyboard();

public slots:
    void enlarge();
    void shrink();

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

};

#endif // XYVIRTUALKEYBOARD_H
