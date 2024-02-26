#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QSqlDatabase>

//注册界面
namespace Ui {
class Signup;
}

class Signup : public QWidget
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

private slots:
    void on_signup_clicked();//注册按钮按下后触发的槽函数

    void on_back_clicked();//返回按钮按下后触发的槽函数

    void on_password_show_stateChanged(int arg1);//密码显示模式勾选框状态改变时触发的槽函数

    void on_ensure_password_show_stateChanged(int arg1);//确认密码显示模式勾选框状态改变时触发的槽函数

    void on_checktype_stateChanged(int arg1);//是否为商家的勾选框状态改变时触发的槽函数

private:
    Ui::Signup *ui;
    QSqlDatabase m_dbSignup;
};

#endif // SIGNUP_H
