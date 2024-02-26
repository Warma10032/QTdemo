#ifndef THIRDWINDOW_TOC_H
#define THIRDWINDOW_TOC_H

#include <QMainWindow>
#include <QSqlDatabase>

//顾客的第三个页面，即订单界面
namespace Ui {
class thirdwindow_toc;
}

class thirdwindow_toc : public QMainWindow
{
    Q_OBJECT

public:
    explicit thirdwindow_toc(QString ,QString, QWidget *parent = nullptr);
    ~thirdwindow_toc();
    void setStoreName(QString );//记录店铺名
    void setPayment(int );//记录总账单
    void setUsername(QString );//记录用户名

private slots:
    void on_pushButton_clicked();//按下确认付款后触发的槽函数

signals:
    void closeSecondwindow();//完成付款后回到顾客主页面，即选择店铺页面的信号


private:
    Ui::thirdwindow_toc *ui;
    QSqlDatabase m_dbbill;
    QString m_strStoreName,m_strUsername;
    int m_iPayment;
};

#endif // THIRDWINDOW_TOC_H
