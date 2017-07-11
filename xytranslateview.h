#ifndef XYTRANSLATEVIEW_H
#define XYTRANSLATEVIEW_H

#include "xybordershadowwidget.h"
#include "xytranslatemodel.h"

class XYTranslateDelegate;
class XYTranslateView : public XYBorderShadowWidget
{
    Q_OBJECT
public:
    explicit XYTranslateView(QWidget *parent = 0);
    ~XYTranslateView();
    void appendData(const QList<XYTranslateItem *> &list);
    void prependData(const QList<XYTranslateItem *> &list);
    void setData(const QList<XYTranslateItem *> &list);
    void clear();
    void setFont(const QFont &font);
    void nextPage();
    void prePage();
    int  itemCount();
    QString getData(int index);    // 通过数字键选择输入

public slots:
    void show();

protected:
    void paintEvent(QPaintEvent *event);

private:
    XYTranslateModel *mopModel;
    QFont moFont;

    int miCurrentPage;
    int miMaxVisibleItem;

    friend class XYInput;
};

#endif // XYTRANSLATEVIEW_H
