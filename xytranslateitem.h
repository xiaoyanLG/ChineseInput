#ifndef XYTRANSLATEITEM_H
#define XYTRANSLATEITEM_H

#include <QString>

class XYTranslateItem
{
public:
    XYTranslateItem(const QString &source = "", // 输入源
                    const QString &translate = "", // 翻译（比如拼音对应的中文）
                    const QString &complete = "", // 完整的拼音（比如中文对应的拼音）
                    const QString &extra = "", // 额外的信息（比如英文翻译）
                    int times = 1, // 词频
                    bool stick = false); // 是否置顶
    ~XYTranslateItem();

private:
    QString  msSource;
    QString  msTranslate;
    QString  msComplete;
    QString  msExtra;
    int      miTimes;
    bool     mbStick;

    friend class XYInput;
    friend class XYTranslateModel;
    friend class XYTranslateView;
    friend class XYDatabaseOperation;
};

#endif // XYTRANSLATEITEM_H
