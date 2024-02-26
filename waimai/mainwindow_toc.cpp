#include "mainwindow_toc.h"
#include "ui_mainwindow_toc.h"
#include "store.h"
#include "login.h"

#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

//顾客的主页面，即顾客选择店铺的页面
MainWindow_toc::MainWindow_toc(QString t_strUsername,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_toc)
{

    ui->setupUi(this);
    setUsername(t_strUsername);

    //设置工具栏的退出登录按钮与关闭该页面，打开上一页面的槽函数连接
    connect(ui->action,SIGNAL(triggered(bool)),new Login,SLOT(show()));
    connect(ui->action,SIGNAL(triggered(bool)),this,SLOT(close()));

    //设置可以滚动的店铺待选框,实现方法：先在ui里放一个scrollArea滚动框，再在框里添加一个widget（addwidget），再在widget中添加一个QVBoxLayout布局（addlayout）
    //再将一个个store子模块添加到layout中，即可实现动态添加自定义控件至滚动框中，而自定义控件自动形成垂直布局。还要设置每个自定义控件的最小大小，避免相互重叠掩盖
    QWidget* widget=new QWidget;
    ui->m_scrollArea->setWidget(widget);
    ui->m_scrollArea->setWidgetResizable(true);

    QVBoxLayout* m_Layout=new QVBoxLayout;
    widget->setLayout(m_Layout);

    //连接数据库到waimai.db
    m_dbstore=QSqlDatabase::addDatabase("QSQLITE","mainwindow_toc_connection");
    m_dbstore.setDatabaseName("waimai.db");
    if(!m_dbstore.open())
    {
        qDebug()<<"open error!"<<m_dbstore.lastError();
    }

    //获取store表中的数据，该表中有先前注册的所有商家的账号，包含这些商家的店铺信息，将这些信息输出，可以用来生成顾客选择店铺的页面
    QSqlQuery query(m_dbstore);
    QString select_all ="select * from store";//从store表中获取所有信息
    if(!query.exec(select_all))
    {
        qDebug()<<"select error!"<<m_dbstore.lastError();
    }
    //一步一步next()，将所有店铺按顺序输出到屏幕上
    else
    {
        while(query.next())
        {
            store* t_ptrStore=new store(m_strUsername,this);//每个店铺输出的信息在一个子store模块上，包含其店铺图片，名称，地址，人均价格和评分
            t_ptrStore->setName(query.value("storename").toString());
            t_ptrStore->setAddress(query.value("address").toString());
            t_ptrStore->setPhoto(query.value("photo").toString());
            t_ptrStore->setPrice(18);
            t_ptrStore->setScore(4.7);
            m_Layout->addWidget(t_ptrStore);
        }
    }




}

MainWindow_toc::~MainWindow_toc()
{
    delete ui;
    m_dbstore.close();
}
//在顾客界面每个窗口直接有一个固定信息的向下传递，即该顾客的用户名。从login登录界面获取后，就会一直向下面的新窗口传递，因为记录用户名才能在点餐时将餐品和用户对应上
//在商家后台也更好显示，所以向下基本有m_strUsername这个成员变量
void MainWindow_toc::setUsername(QString t_str)
{
    m_strUsername=t_str;
    qDebug()<<m_strUsername;
}


