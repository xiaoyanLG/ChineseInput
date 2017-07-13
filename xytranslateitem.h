#ifndef XYTRANSLATEITEM_H
#define XYTRANSLATEITEM_H

#include <QString>

class XYTranslateItem
{
public:
    XYTranslateItem(const QString &source = "", // 输入源
                    const QString &translate = "", // 存放当前想输入的类型（比如中文，或者英文）
                    const QString &complete = "", // 完整的拼音（比如中文对应的拼音，或者完整的）
                    const QString &extra = "", // 额外的信息（比如中文词组的字数）
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
