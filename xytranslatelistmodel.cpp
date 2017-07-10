#include "xytranslatelistmodel.h"

XYTranslateListModel::XYTranslateListModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

XYTranslateListModel::~XYTranslateListModel()
{

}

void XYTranslateListModel::addData(const QList<XYTranslateItem *> &list)
{
    mlistFindTranslates += list;
}

void XYTranslateListModel::setData(const QList<XYTranslateItem *> &list)
{
    qDeleteAll(mlistFindTranslates);
    mlistFindTranslates.clear();
    mlistFindTranslates = list;
}

int XYTranslateListModel::rowCount(const QModelIndex &parent) const
{
    return mlistFindTranslates.size();
}

QVariant XYTranslateListModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        return mlistFindTranslates.at(index.row())->msSource;
    default:
        break;
    }

    return QVariant();
}

