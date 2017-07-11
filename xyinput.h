#ifndef XYINPUT_H
#define XYINPUT_H

#include "xybordershadowwidget.h"
#include "xytranslateview.h"

class QLineEdit;
class XYInput : public XYBorderShadowWidget
{
    Q_OBJECT
public:
    static XYInput *getInstance();
    explicit XYInput(QWidget *parent = 0);
    bool initInputBase(const QString &path);
    ~XYInput();

private slots:
    void mslotFindTranslate(const QString &keyword); // 查找输入内容对应的词（中英文都通过这个接口）
    void close();
    void show();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void load();           // 装载查找到的内容，并显示出来

private:
    static XYInput *mopInstance;
    QWidget   *mopLatestWidget;
    QLineEdit *mopLineEdit;

    QHash<QString, QString> pyChineseHash;
    XYTranslateView *mopTransLateView;
};

#endif // XYINPUT_H
