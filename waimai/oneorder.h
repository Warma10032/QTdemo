#ifndef ONEORDER_H
#define ONEORDER_H

#include <QWidget>
#include <QSqlDatabase>

//店铺订单的单个模块
namespace Ui {
class oneOrder;
}

class oneOrder : public QWidget
{
    Q_OBJECT

public:
    explicit oneOrder(QString, QString, QWidget *parent = nullptr);
    ~oneOrder();
    void showOrder(QString );//订单字符串显示到文本框的函数
    void setUsername(QString );//记录用户名的函数
    void setStorename(QString );//记录店铺名的函数

private slots:
    void on_complete_clicked();//完成订单按钮按下后触发的槽函数

private:
    Ui::oneOrder *ui;
    QString m_strUsername,m_strStorename;
    QSqlDatabase m_dboneorder;
};

#endif // ONEORDER_H
