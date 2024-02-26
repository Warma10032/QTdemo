#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QSqlDatabase>

//登录界面
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_login_clicked();//登录按钮按下后触发的槽函数

    void on_signup_clicked();//注册按钮按下后触发的槽函数

    void on_password_show_stateChanged(int arg1);//更改密码显示的勾选框改变时触发的槽函数

private:
    Ui::Login *ui;
    QSqlDatabase m_dbLogin;
};

#endif // LOGIN_H
