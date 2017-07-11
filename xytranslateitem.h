#ifndef XYTRANSLATEITEM_H
#define XYTRANSLATEITEM_H

#include <QString>

class XYTranslateItem
{
public:
    XYTranslateItem(const QString &source = "",
                    const QString &translate = "",
                    const QString &extra = "");
    ~XYTranslateItem();

private:
    QString  msSource;
    QString  msTranslate;
    QString  msExtra;

    friend class XYTranslateModel;
    friend class XYTranslateDelegate;
};

#endif // XYTRANSLATEITEM_H
