#ifndef HOME_TOB_H
#define HOME_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>

//店铺的个人中心页面
namespace Ui {
class home;
}

class home : public QMainWindow
{
    Q_OBJECT

public:
    explicit home(QString ,QWidget *parent = nullptr);
    ~home();
    void setUsername(QString );//记录用户名的函数
    void setStorename(QString );//记录店铺名的函数
    void refresh();//刷新个人中心数据显示的函数
private slots:
    void on_change_clicked();//更改个人中心数据按钮按下后触发的槽函数

signals:
    void closewindow();//发送更改完消息后关闭主页面返回登录页面的信号

private:
    Ui::home *ui;
    QSqlDatabase m_dbhome;
    QString m_strUsername,m_strStorename;
};

#endif // HOME_TOB_H
