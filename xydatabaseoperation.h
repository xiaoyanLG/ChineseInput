#ifndef XYDATABASEOPERATION_H
#define XYDATABASEOPERATION_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>

class XYTranslateItem;
class XYDatabaseOperation : public QObject
{
    Q_OBJECT
public:
    static XYDatabaseOperation *getInstance();
    explicit XYDatabaseOperation(QObject *parent = 0);
    ~XYDatabaseOperation();

    bool createDatabaseFile(const QString &filePath, const QString &passwd = "", bool fource = false);
    bool openDatabaseFile(const QString &filePath, const QString &passwd = "");

    bool createInputTable();
    bool insertData(const QList<XYTranslateItem *> &list, const QString &table);
    QList<XYTranslateItem *> findData(const QString &key,
                                      const QString &table);

private:
    static XYDatabaseOperation *DB;
};

#define XYDB XYDatabaseOperation::getInstance()
#endif // XYDATABASEOPERATION_H
