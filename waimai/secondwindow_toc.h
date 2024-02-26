#ifndef SECONDWINDOW_TOC_H
#define SECONDWINDOW_TOC_H

#include <QMainWindow>
#include <QSqlDatabase>

//顾客的第二页面，即顾客选餐页面
namespace Ui {
class secondwindow_toc;
}

class secondwindow_toc : public QMainWindow
{
    Q_OBJECT

public:
    explicit secondwindow_toc(QString ,QString ,QWidget *parent = nullptr);
    ~secondwindow_toc();
    void setStoreName(QString );//记录店铺名的函数
    void setUsername(QString );//记录用户名的函数

private slots:
    void calPayment();//计算总价格的槽函数

    void on_pushButton_clicked();//按下确定订单按钮后触发的槽函数，进入下一个订单页面

private:
    Ui::secondwindow_toc *ui;
    QSqlDatabase m_dbdishes;
    QString m_strStoreName,m_strUsername;
    int m_iPayment=0;
};

#endif // SECONDWINDOW_TOC_H
