#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>

namespace Ui {
class LoginView;
}

class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private slots:
    void on_btn_Login_clicked();

private:
    Ui::LoginView *ui;

signals:
    void loginSuccess();
    void loginFailed();
};


#endif // LOGINVIEW_H
