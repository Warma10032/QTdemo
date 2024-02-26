#ifndef STORE_H
#define STORE_H

#include <QWidget>
#include <QSqlDatabase>

//店铺的子模块，用于显示店铺信息
namespace Ui {
class store;
}

class store : public QWidget
{
    Q_OBJECT

public:
    explicit store(QString ,QWidget *parent = nullptr);
    void setName(QString ="未知的小吃店");//记录店铺的名字并显示在窗口上
    void setScore(double =5.0);//记录店铺的评分并显示在窗口上
    void setPrice(double =0.0);//记录店铺的人均价格并显示在窗口上
    void setAddress(QString = "前面的区域，以后再来探索吧");//记录店铺的地址并显示在窗口上
    void setPhoto(QString );
    ~store();
    void setUsername(QString );//记录顾客用户名并向下传递


private slots:
    void on_select_clicked();//每个店铺子模块有一个按钮，按了这个按钮后触发的槽函数代表顾客选择了这个店铺，顾客将被带入这个店铺的子页面，让顾客在该店铺选餐。

private:
    Ui::store *ui;
    QString m_strName,m_strAddress,m_strUsername,m_strPhoto;//记录QString的变量，包括店铺的名称，地址，和顾客的用户名
    double m_dScore,m_dPrice;//记录double的成员变量，包括店铺的评分和人均售价
    QSqlDatabase m_dbStore;
};

#endif // STORE_H
