#ifndef INCOME_TOB_H
#define INCOME_TOB_H

#include <QMainWindow>
#include <QSqlDatabase>

//店铺营收页面
namespace Ui {
class Income;
}

class Income : public QMainWindow
{
    Q_OBJECT

public:
    explicit Income(QString, QWidget *parent = nullptr);
    ~Income();
    void setStorename(QString );//记录店铺名的函数

private:
    Ui::Income *ui;
    QSqlDatabase m_dbIncome;
    QString m_strStorename;
};

#endif // INCOME_TOB_H
