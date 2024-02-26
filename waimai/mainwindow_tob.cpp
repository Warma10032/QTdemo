#include "mainwindow_tob.h"
#include "ui_mainwindow_tob.h"
#include "menu_tob.h"
#include "order_tob.h"
#include "income_tob.h"
#include "home_tob.h"
#include "login.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

//商家主页面
MainWindow_tob::MainWindow_tob(QString t_strUsername, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_tob)
{
    //设记录用户名并将store表中对应的店铺名记录到数据成员中
    setStoreName(t_strUsername);
    ui->setupUi(this);

    //将菜单栏退出登录按钮与关闭该页面和新开登录界面相连接
    connect(ui->menu,SIGNAL(aboutToShow()),new Login,SLOT(show()));
    connect(ui->menu,SIGNAL(aboutToShow()),this,SLOT(close()));

    //设置上方标签为店铺名字
    ui->label->setText(m_strStoreName);

    //打开数据库
    m_dbtob=QSqlDatabase::addDatabase("QSQLITE");
    m_dbtob.setDatabaseName("waimai.db");
    if(!m_dbtob.open())
    {
        qDebug()<<"open error: "<<m_dbtob.lastError();
    }
}

MainWindow_tob::~MainWindow_tob()
{
    delete ui;
    m_dbtob.close();
}

void MainWindow_tob::setStoreName(QString t_str)
{
    setUsername(t_str);//记录用户名
    QSqlQuery query(m_dbtob);
    if(!query.exec(QString("select storename from store where username = '%1'").arg(t_str)))//从store表中读取店铺名记录到成员变量中
    {
        qDebug()<<"select error!"<<m_dbtob.lastError();
    }
    else
    {
        query.next();
        m_strStoreName = query.value("storename").toString();//将store表中对应的店铺名记录到数据成员中
    }

}

void MainWindow_tob::setUsername(QString t_str)
{
    m_strUsername=t_str;
}

//菜单管理按钮按下后触发的槽函数
void MainWindow_tob::on_pushButton_clicked()
{
    Menu *menu=new Menu(m_strStoreName,this);//打开一个菜单界面
    menu->show();
}

//订单管理按钮按下后触发的槽函数
void MainWindow_tob::on_pushButton_2_clicked()
{
    order_tob *order =new order_tob(m_strStoreName,this);//打开一个订单查看界面
    order->show();
}

//营收查看按钮按下后触发的槽函数
void MainWindow_tob::on_pushButton_3_clicked()
{
    Income *income=new Income(m_strStoreName,this);//打开一个营收查看界面
    income->show();
}

//个人中心按钮按下后触发的槽函数
void MainWindow_tob::on_pushButton_4_clicked()
{
    home *h=new home(m_strUsername,this);//打开一个个人主页
    //个人主页进行修改后，需要更新数据库，因此将程序返回至登录界面，需要重新登录
    connect(h,SIGNAL(closewindow()),new Login,SLOT(show()));//将确认修改按钮与新开登陆界面连接
    connect(h,SIGNAL(closewindow()),this,SLOT(close()));//将确认修改与当前主页面关闭连接
    h->show();
}

