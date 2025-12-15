#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

class IDatabase : public QObject
{
    Q_OBJECT
public:
    // 1. 获取单例的静态方法
    static IDatabase& getInstance();

    // 3. 初始化并连接数据库的方法
    bool initDatabase();

    // 获取当前数据库连接名称（可选）
    QString userLogin(QString username, QString password); // 示例业务函数
    // 添加一个验证用户的函数，返回 true 表示验证通过
    bool validateUser(QString username, QString password);

private:
    // 2. 构造函数私有化
    explicit IDatabase(QObject *parent = nullptr);
    ~IDatabase();

    // 4. 禁止拷贝和赋值 (C++11 特性)
    IDatabase(const IDatabase&) = delete;
    IDatabase& operator=(const IDatabase&) = delete;

    // 数据库对象
    QSqlDatabase database;
};

#endif // IDATABASE_H
