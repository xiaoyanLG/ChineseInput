#include "xytranslatemodel.h"

XYTranslateModel::XYTranslateModel(QObject *parent)
    :QObject(parent)
{

}

XYTranslateModel::~XYTranslateModel()
{

}

void XYTranslateModel::addData(const QList<XYTranslateItem *> &list)
{
    mlistFindTranslates += list;
}

void XYTranslateModel::setData(const QList<XYTranslateItem *> &list)
{
    clear();
    mlistFindTranslates = list;
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
    case EXTRA:
        return mlistFindTranslates.at(index)->msExtra;
    default:
        return QString();
    }
}

