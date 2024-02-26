#ifndef MAINWINDOW_TOC_H
#define MAINWINDOW_TOC_H

#include <QMainWindow>
#include <QSqlDatabase>

//顾客的主页面，即顾客选择店铺的页面
namespace Ui {
class MainWindow_toc;
}

class MainWindow_toc : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_toc(QString ,QWidget *parent = nullptr);
    ~MainWindow_toc();
    void setUsername(QString );//记录用户名的函数


private:
    Ui::MainWindow_toc *ui;
    QSqlDatabase m_dbstore;
    QString m_strUsername;
};

#endif // MAINWINDOW_TOC_H
