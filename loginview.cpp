#include "loginview.h"
#include "ui_loginview.h"
#include "idatabase.h"  // 1. 引入数据库单例
#include <QMessageBox>  // 2. 引入消息框

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}



void LoginView::on_btn_Login_clicked()
{
    // 1. 获取界面上输入的账号密码
    // 注意：请确认 .ui 文件里输入框的 objectName 是不是叫 lineEdit_UserName 和 lineEdit_Password
    QString username = ui->lineEdit_UserName->text();
    QString password = ui->lineEdit_Password->text();

    // 2. 简单的非空校验
    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名或密码不能为空");
        return;
    }

    // 3. 调用数据库单例进行验证
    bool loginOk = IDatabase::getInstance().validateUser(username, password);

    // 4. 根据结果处理
    if (loginOk) {
        // 验证成功
        emit loginSuccess(); // 发送信号，MasterView 会收到并跳转页面
    } else {
        // 验证失败
        QMessageBox::critical(this, "登录失败", "用户名或密码错误，请重试！");
        emit loginFailed(); // 可选：发送失败信号
    }
}
