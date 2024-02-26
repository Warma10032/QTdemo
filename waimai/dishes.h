#ifndef DISHES_H
#define DISHES_H

#include <QWidget>
#include <QSqlDatabase>

//单个菜品模块
namespace Ui {
class dishes;
}

class dishes : public QWidget
{
    Q_OBJECT

public:
    explicit dishes(QWidget *parent = nullptr);
    ~dishes();
    void setName(QString = "西红柿炒蛋");//设置菜品的名称
    void setIntroduction(QString = "好吃");//设置菜品的介绍
    void setPrice(int );//设置菜品的单价
    void setStoreName(QString );//记录店铺的名称
    void setPhoto(QString );//设置菜品的图片

signals:
    void signaltocalPayment();//发出实时计算总价的信号

public slots:
    void on_quantity_valueChanged(int arg1);//当选餐数量发生改变时触发的槽函数

private:
    Ui::dishes *ui;
    QString m_strName,m_strIntroduction,m_strStoreName,m_strPhoto;
    int m_iPrice;
    int m_quantity;
    QSqlDatabase m_dbdish;
};

#endif // DISHES_H
