#include "masterview.h"
#include <QApplication>
#include "idatabase.h"  // 1. 引入数据库头文件
#include <QMessageBox>  // 2. 引入消息框，用于报错

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 3. 在创建窗口之前，先初始化数据库
    if (!IDatabase::getInstance().initDatabase()) {
        // 如果连接失败，弹出严重错误提示框
        QMessageBox::critical(nullptr, "数据库错误", "无法连接数据库，请检查数据库文件是否存在！");

        // 返回非0值，直接退出程序，不显示主界面
        return -1;
    }

    // 4. 数据库连接成功后，再加载主界面
    MasterView w;
    w.show();

    return a.exec();
}
