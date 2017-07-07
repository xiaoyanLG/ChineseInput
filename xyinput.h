#ifndef XYINPUT_H
#define XYINPUT_H

#include "xymovablewidget.h"
#include "xybordershadowwidget.h"

class QLineEdit;
class XYInput : public XYMovableWidget, public XYBorderShadowWidget
{
    Q_OBJECT
public:
    static XYInput *getInstance();
    explicit XYInput(QWidget *parent = 0);
    ~XYInput();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    static XYInput *mopInstance;
    QWidget   *mopLatestWidget;
    QLineEdit *mopLineEdit;
};

#endif // XYINPUT_H
