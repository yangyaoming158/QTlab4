#include "idatabase.h"
#include <QCoreApplication> // 用于获取应用程序路径
#include <QFile>
#include <QSqlQuery>

// 构造函数：这里可以做一些简单的初始化，但不要进行耗时的连接操作
IDatabase::IDatabase(QObject *parent)
    : QObject{parent}
{
    // 构造函数留空或做基础变量初始化
}

// 析构函数：关闭数据库
IDatabase::~IDatabase()
{
    if (database.isOpen()) {
        database.close();
    }
}

// 单例获取方法的实现
IDatabase& IDatabase::getInstance()
{
    // 局部静态变量，C++11 保证线程安全，且只初始化一次
    static IDatabase instance;
    return instance;
}

// 初始化数据库连接
bool IDatabase::initDatabase()
{
    // 如果已经打开，直接返回 true
    if (database.isOpen()) {
        return true;
    }

    // 1. 添加数据库驱动
    database = QSqlDatabase::addDatabase("QSQLITE");


    // 2. 设置数据库文件路径或连接信息
    QString dbPath = QCoreApplication::applicationDirPath() + "/Lab4Mode.db";
    // 检查文件是否存在
    if (!QFile::exists(dbPath)) {
        qDebug() << "错误：数据库文件不存在，路径是：" << dbPath;
        return false; // 如果文件没找到，直接报错，不要让它自动创建空的
    }
    database.setDatabaseName(dbPath);


    // 3. 打开数据库
    if (!database.open()) {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        return false;
    }

    qDebug() << "Database connection success!";
    return true;
}

bool IDatabase::validateUser(QString username, QString password)
{
    if (!database.isOpen()) {
        if (!initDatabase()) return false; // 如果没开，尝试打开
    }

    QSqlQuery query;
    // 准备 SQL 语句，使用 :user 和 :pass 作为占位符，防止 SQL 注入
    query.prepare("SELECT * FROM User WHERE USERNAME = :user AND PASSWORD = :pass");
    query.bindValue(":user", username);
    query.bindValue(":pass", password);

    // 执行查询
    if (query.exec()) {
        // query.next() 返回 true 说明查到了至少一行数据，即用户名密码匹配
        if (query.next()) {
            return true;
        }
    } else {
        qDebug() << "Login query failed:" << query.lastError();
    }

    return false; // 没查到或者出错，返回失败
}


