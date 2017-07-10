#ifndef XYLISTMODEL_H
#define XYLISTMODEL_H

#include <QAbstractListModel>
#include "xytranslateitem.h"

class XYTranslateListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit XYTranslateListModel(QObject *parent = 0);
    ~XYTranslateListModel();
    void addData(const QList<XYTranslateItem *> &list);
    void setData(const QList<XYTranslateItem *> &list);


    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QList<XYTranslateItem *> mlistFindTranslates;

};

#endif // XYLISTMODEL_H
