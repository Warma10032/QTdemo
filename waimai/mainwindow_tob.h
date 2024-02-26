#ifndef MAINWINDOW_TOB_H
#define MAINWINDOW_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>

//商家主页面
namespace Ui {
class MainWindow_tob;
}

class MainWindow_tob : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_tob(QString ,QWidget *parent = nullptr);
    ~MainWindow_tob();
    void setStoreName(QString );//记录店铺名
    void setUsername(QString );//记录用户名

private slots:
    void on_pushButton_clicked();//菜单管理按钮按下后触发的槽函数

    void on_pushButton_2_clicked();//订单管路按钮按下后触发的槽函数

    void on_pushButton_3_clicked();//营收查看按钮按下后触发的槽函数

    void on_pushButton_4_clicked();//个人中心按钮按下后触发的槽函数

private:
    Ui::MainWindow_tob *ui;
    QString m_strStoreName,m_strUsername;
    QSqlDatabase m_dbtob;
};

#endif // MAINWINDOW_TOB_H
