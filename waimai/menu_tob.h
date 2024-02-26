#ifndef MENU_TOB_H
#define MENU_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>

//店铺菜单管理页面
namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QString ,QWidget *parent = nullptr);
    ~Menu();
    void setStoreName(QString );//记录店铺名的函数
    void refresh();//刷新菜单显示的函数

private slots:
    void on_pushButton_clicked();//添加菜品按钮按下后触发的槽函数

    void on_pushButton_2_clicked();//按下刷新按钮后触发的槽函数（应对bug）

    void on_pushButton_3_clicked();//按下删除按钮后触发的槽函数

    void on_pushButton_4_clicked();//修改按钮按下后触发的槽函数

private:
    Ui::Menu *ui;
    QSqlDatabase m_dbMenu;
    QString m_strStoreName;
};

#endif // MENU_TOB_H
