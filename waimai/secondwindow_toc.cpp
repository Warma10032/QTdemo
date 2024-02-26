#include "secondwindow_toc.h"
#include "ui_secondwindow_toc.h"
#include "dishes.h"
#include "thirdwindow_toc.h"

#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

//顾客的第二页面，即顾客选餐页面
secondwindow_toc::secondwindow_toc(QString t_strUsername,QString t_strStoreName,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::secondwindow_toc)
{
    ui->setupUi(this);

    //记录用户名和店铺名都是为了在生成订单的时候将顾客、订单与商家三者相互对应，一份订单需要知道甲方和乙方分别是谁。
    setUsername(t_strUsername);//记录顾客用户名，用于向下传递
    setStoreName(t_strStoreName);//记录店铺名，用于向下传递

    this->setWindowTitle(m_strStoreName);

    //将返回按钮与关闭本窗口绑定
    connect(ui->menu,SIGNAL(aboutToShow()),this,SLOT(close()));


    //连接数据库waimai.db
    m_dbdishes=QSqlDatabase::addDatabase("QSQLITE","dishes_connection");
    m_dbdishes.setDatabaseName("waimai.db");
    if(!m_dbdishes.open())
    {
        qDebug()<<"open error!"<<m_dbdishes.lastError();
    }

    //同mainwindow_toc展示多个店铺，这里是展示一个店铺里的多个菜品
    //设置可以滚动的菜品待选框,实现方法：先在ui里放一个scrollArea滚动框，再在框里添加一个widget（addwidget），再在widget中添加一个QVBoxLayout布局（addlayout）
    //再将一个个store子模块添加到layout中，即可实现动态添加自定义控件至滚动框中，而自定义控件自动形成垂直布局。还要设置每个自定义控件的最小大小，避免相互重叠掩盖
    QWidget* widget=new QWidget;
    QVBoxLayout* m_Layout=new QVBoxLayout;
    widget->setLayout(m_Layout);

    ui->m_scrollArea->setWidget(widget);
    ui->m_scrollArea->setWidgetResizable(true);//设置滚动框中的widget可以滚动

    QSqlQuery query(m_dbdishes);

    //一个店铺的菜品信息记录在了以该店铺名命名的表中，因此这里要从m_strStoreName中选择数据
    if(!query.exec(QString("select * from %1").arg(m_strStoreName)))
    {
        qDebug()<<"select error!"<<m_dbdishes.lastError();
    }
    //一步步next()，将数据库中的菜品数据提取并展示出来
    else
    {
        while(query.next())
        {
            dishes* t_ptr=new dishes(this);//每一个菜品对应一个dish子模块
            //设置该子模块对应的一些信息用来展示
            t_ptr->setName(query.value("name").toString());//设置菜品的名称
            t_ptr->setIntroduction(query.value("introduction").toString());//设置菜品的介绍
            t_ptr->setPrice(query.value("price").toInt());//设置菜品的价格
            t_ptr->setStoreName(this->m_strStoreName);
            t_ptr->setPhoto(query.value("photo").toString());
            m_Layout->addWidget(t_ptr);
            //在选餐界面中，我设计了一个实时计算总账单的lcd number显示，所以当顾客选餐变动时，就要立刻重算总账单。
            //因此设置了一个在选餐变动时就发出信号SIGNAL(signaltocalPayment())，接收到信号后就调用SLOT(calPayment())槽函数进行计算账单总价，并显示在lcd number上
            QObject::connect(t_ptr,SIGNAL(signaltocalPayment()),this,SLOT(calPayment()));
        }
    }



}

secondwindow_toc::~secondwindow_toc()
{
    delete ui;
}

//记录店铺名，用于对应订单
void secondwindow_toc::setStoreName(QString t_str)
{
    m_strStoreName=t_str;
}

//实时计算目选择的餐品总价，并显示在lcd number上
void secondwindow_toc::calPayment()
{
    QSqlQuery query(m_dbdishes);
    //当顾客选餐数量发生改变时，当前选餐数据会存入临时数据字段quantity中，因此在此查找所有quantity大于0对应的价格进行计算
    if(!query.exec(QString("select price,quantity from %1 where quantity >0").arg(m_strStoreName)))
    {
        qDebug()<<"select error!"<<m_dbdishes.lastError();
    }
    //重新计算价格
    else
    {
        m_iPayment=0;//清空总价
        //计算总价，单价*数量
        while(query.next())
        {
            m_iPayment+=query.value("price").toInt()*query.value("quantity").toInt();
        }
        ui->payment->display(m_iPayment);//在lcd number中展示
    }
}


void secondwindow_toc::on_pushButton_clicked()//按下确定按钮后触发的槽函数，进入thirdwindow_toc界面，即用户订单付款界面
{
    thirdwindow_toc *tw=new thirdwindow_toc(m_strUsername,m_strStoreName,this);//实例化一个订单付款界面
    connect(tw,SIGNAL(closeSecondwindow()),this,SLOT(close()));//将付款页面（第三页面）的确定按钮与选餐页面（第二页面）的关闭相连接
    if(m_iPayment!=0)
    {
        tw->setPayment(m_iPayment);//将先前已计算的订单总价传入订单付款界面,可减少重复计算
    }
    tw->show();
}

void secondwindow_toc::setUsername(QString t_str)//记录顾客用户名
{
    m_strUsername=t_str;
    qDebug()<<m_strUsername;
}

