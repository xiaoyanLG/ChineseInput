#include "xydatabaseoperation.h"
#include "xytranslateitem.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QVariant>
XYDatabaseOperation *XYDatabaseOperation::DB = NULL;
XYDatabaseOperation *XYDatabaseOperation::getInstance()
{
    if (DB == NULL)
    {
        DB = new XYDatabaseOperation;
    }
    return DB;
}

XYDatabaseOperation::XYDatabaseOperation(QObject *parent)
    : QObject(parent)
{

}

XYDatabaseOperation::~XYDatabaseOperation()
{

}

bool XYDatabaseOperation::createDatabaseFile(const QString &filePath, const QString &passwd, bool fource)
{
    if (QFile::exists(filePath)) // 文件存在
    {
        if (fource)
        {
            if (!QFile::remove(filePath)) // 删除失败也返回错误
            {
                return false;
            }
        }
        else    // 不强制创建返回错误
        {
            return false;
        }
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("SQLITECIPHER", "XYInout");
    db.setDatabaseName(filePath);
    db.setPassword(passwd);
    db.setConnectOptions("QSQLITE_CREATE_KEY");
    return db.open();
}

bool XYDatabaseOperation::openDatabaseFile(const QString &filePath, const QString &passwd)
{
    if (QSqlDatabase::database("XYInout").isOpen())
    {
        return true;
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("SQLITECIPHER", "XYInout");
    db.setDatabaseName(filePath);
    db.setPassword(passwd);
    return db.open();
}

bool XYDatabaseOperation::createInputTable()
{
    QSqlQuery query(QSqlDatabase::database("XYInout"));

    // 单字表
    bool ok = query.exec("CREATE TABLE IF NOT EXISTS  singlePingying ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                         "pingying VARCHAR NOT NULL,"
                         "chinese VARCHAR NOT NULL,"
                         "extra VARCHAR NULL,"
                         "times INTEGER NOT NULL,"
                         "stick BOOL NULL)");
    if (!ok)
    {
        return false;
    }

    // 基础表
    ok = query.exec("CREATE TABLE IF NOT EXISTS  basePintying ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "pingying VARCHAR NOT NULL,"
                    "chinese VARCHAR NOT NULL,"
                    "extra VARCHAR NULL,"
                    "times INTEGER NOT NULL,"
                    "stick BOOL NULL)");
    if (!ok)
    {
        return false;
    }

    // 用户表
    ok = query.exec("CREATE TABLE IF NOT EXISTS  userPingying ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "pingying VARCHAR NOT NULL,"
                    "chinese VARCHAR NOT NULL,"
                    "extra VARCHAR NULL,"
                    "times INTEGER NOT NULL,"
                    "stick BOOL NULL)");
    if (!ok)
    {
        return false;
    }

    // 英文表
    ok = query.exec("CREATE TABLE IF NOT EXISTS  englishTable ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "english VARCHAR NOT NULL,"
                    "translate VARCHAR NOT NULL,"
                    "extra VARCHAR NULL,"
                    "times INTEGER NOT NULL,"
                    "stick BOOL NULL)");
    if (!ok)
    {
        return false;
    }

    // 英文用户表
    ok = query.exec("CREATE TABLE IF NOT EXISTS  userEnglishTable ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "english VARCHAR NOT NULL,"
                    "translate VARCHAR NOT NULL,"
                    "extra VARCHAR NULL,"
                    "times INTEGER NOT NULL,"
                    "stick BOOL NULL)");
    if (!ok)
    {
        return false;
    }
    return ok;
}

bool XYDatabaseOperation::insertData(const QList<XYTranslateItem *> &list, const QString &table)
{
    QString field1, field2;
    if (table.contains("english"))
    {
        field1 = "english";
        field2 = "translate";
    }
    else
    {
        field1 = "pingying";
        field2 = "chinese";
    }
    QSqlDatabase::database("XYInout").transaction();   //开始一个事务
    QSqlQuery query(QSqlDatabase::database("XYInout"));
    bool ok = true;
    for (int i = 0; i < list.size(); ++i)
    {
        query.prepare(QString("INSERT INTO %1 (id, %2, %3, extra, times, stick) "
                              "VALUES (:id, :%4, :%5, :extra, :times, :stick)")
                      .arg(table)
                      .arg(field1)
                      .arg(field2)
                      .arg(field1)
                      .arg(field2));
        query.bindValue(QString(":%1").arg(field1), list.at(i)->msComplete);
        query.bindValue(QString(":%1").arg(field2), list.at(i)->msTranslate);
        query.bindValue(":extra", list.at(i)->msExtra);
        query.bindValue(":times", list.at(i)->miTimes);
        query.bindValue(":stick", list.at(i)->mbStick);
        ok = query.exec();
        if (!ok)
        {
            break;
        }
    }
    if (ok)
    {
        QSqlDatabase::database("XYInout").commit();
    }
    else
    {
        QSqlDatabase::database("XYInout").rollback();
    }
    return ok;
}

QList<XYTranslateItem *> XYDatabaseOperation::findData(const QString &key, const QString &number, const QString &table)
{
    QString field1, field2;
    if (table.contains("english"))
    {
        field1 = "english";
        field2 = "translate";
    }
    else
    {
        field1 = "pingying";
        field2 = "chinese";
    }
    QSqlQuery query(QSqlDatabase::database("XYInout"));
    query.exec(QString("SELECT id, %1, %2, extra, times, stick FROM %3 "
                       "WHERE %4 like \"%5\" AND extra like \"%6\" "
                       "ORDER BY times")
               .arg(field1)
               .arg(field2)
               .arg(table)
               .arg(field1)
               .arg(key)
               .arg(number));

    QList<XYTranslateItem *> list;
    while (query.next())
    {
        list.append(new XYTranslateItem("",
                                        query.value(2).toString(),
                                        query.value(1).toString(),
                                        query.value(3).toString(),
                                        query.value(4).toInt(),
                                        query.value(5).toBool()));
    }
    return list;
}

