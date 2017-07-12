#ifndef XYTRANSLATEITEM_H
#define XYTRANSLATEITEM_H

#include <QString>

class XYTranslateItem
{
public:
    XYTranslateItem(const QString &source = "", // 输入源
                    const QString &translate = "", // 翻译（比如拼音对应的中文）
                    const QString &complete = "", // 完整的拼音（比如拼音对应的中文）
                    const QString &extra = "", // 额外的信息（比如英文翻译）
                    int times = 1); // 词频
    ~XYTranslateItem();

private:
    QString  msSource;
    QString  msTranslate;
    QString  msComplete;
    QString  msExtra;
    int      miTimes;

    friend class XYTranslateModel;
    friend class XYTranslateView;
};

#endif // XYTRANSLATEITEM_H
