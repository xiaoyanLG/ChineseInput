#include "xytranslatemodel.h"

XYTranslateModel::XYTranslateModel(QObject *parent)
    :QObject(parent)
{

}

XYTranslateModel::~XYTranslateModel()
{

}

void XYTranslateModel::appendData(const QList<XYTranslateItem *> &list)
{
    mlistFindTranslates += list;
}

void XYTranslateModel::prependData(const QList<XYTranslateItem *> &list)
{
    mlistFindTranslates = list + mlistFindTranslates;
}

void XYTranslateModel::setData(const QList<XYTranslateItem *> &list)
{
    clear();
    mlistFindTranslates = list;
}

void XYTranslateModel::delItem(XYTranslateItem *item)
{
    delete item;
    mlistFindTranslates.removeAll(item);
}

void XYTranslateModel::stickItem(XYTranslateItem *item)
{
    mlistFindTranslates.removeAll(item);
    mlistFindTranslates.prepend(item);
}

XYTranslateItem *XYTranslateModel::getItem(int index)
{
    if (mlistFindTranslates.size() > index)
    {
        return mlistFindTranslates.at(index);
    }
    else
    {
        return NULL;
    }
}

void XYTranslateModel::clear()
{
    qDeleteAll(mlistFindTranslates);
    mlistFindTranslates.clear();
}

bool XYTranslateModel::haveData(int index) const
{
    return index < mlistFindTranslates.size();
}

int XYTranslateModel::counts()
{
    return mlistFindTranslates.size();
}

QString XYTranslateModel::data(int index, int role) const
{
    if (index >= mlistFindTranslates.size())
    {
        return QString();
    }
    switch (role)
    {
    case SOURCE:
        return mlistFindTranslates.at(index)->msSource;
    case TRANSLATE:
        return mlistFindTranslates.at(index)->msTranslate;
    case COMPLETE:
        return mlistFindTranslates.at(index)->msTranslate;
    case EXTRA:
        return mlistFindTranslates.at(index)->msExtra;
    case TIMES:
        return QString::number(mlistFindTranslates.at(index)->miTimes);
    default:
        return QString();
    }
}

