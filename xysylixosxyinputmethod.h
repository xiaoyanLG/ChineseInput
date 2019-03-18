#ifndef XYSYLIXOSXYINPUTMETHOD_H
#define XYSYLIXOSXYINPUTMETHOD_H

#include <private/xyinputmethodinterface.h>
#include "xysylixosxyinputmethod_global.h"
#include <QTimer>

class XYInput;
class XYSYLIXOSINPUTMETHODSHARED_EXPORT XYSylixOSXYInputMethod : public XYInputMethodInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID XYInputMethodInterfaceIID FILE "key.json")
public:
    explicit XYSylixOSXYInputMethod(QObject *parent = 0);

    void init(const QString &dllpath) override;

    QImage icon() override;
    QString describe() override;
    bool isVisible() override;
    void show(QWindow *parent, const QPoint &cursorPos) override;
    void hide() override;
    QRect keyboardRectangle() override;
    QLocale locale() override;
    Qt::LayoutDirection inputDirection() override;

    void initFirst();

    QMenu *iconHandleMenu() override;
    void handleKeyEvent(QKeyEvent *event) override;

public slots:
    void send_keyEvent(QKeyEvent *event);
    void send_preedit(const QString &text);
    void send_commit(const QString &text);

private:
    QWindow   *m_Parent;
    QPoint     m_Pos;
    XYInput   *m_Input = nullptr;

};

#endif // XYSYLIXOSXYINPUTMETHOD_H
