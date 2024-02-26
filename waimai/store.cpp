#include "store.h"
#include "ui_store.h"
#include "secondwindow_toc.h"

#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

//显示店铺的模块
store::store(QString t_str,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::store)
{
    ui->setupUi(this);

    //记录用户名
    setUsername(t_str);

    //连接数据库
    m_dbStore=QSqlDatabase::addDatabase("QSQLITE","store_connection");
    m_dbStore.setDatabaseName("waimai.db");
    if(!m_dbStore.open())
    {
        qDebug()<<"open error!"<<m_dbStore.lastError();
    }
}

void store::setName(QString t_str)//记录店铺的名字并显示在窗口上
{
    m_strName=t_str;
    ui->name->setText(m_strName);
}
void store::setScore(double t_d)//记录店铺的评分并显示在窗口上
{
    m_dScore=t_d;
    ui->score->setText(QString("%1/5.0").arg(t_d));
}
void store::setPrice(double t_d)//记录店铺的均价并显示在窗口上
{
    m_dPrice=t_d;
    ui->price->setText(QString("%1 r/人").arg(t_d));
}

void store::setAddress(QString t_str)//记录店铺的地址并显示在窗口上
{
    m_strAddress=t_str;
    ui->address->setText(m_strAddress);
    ui->address->setAlignment(Qt::AlignRight);
}
void store::setPhoto(QString t_str)//记录店铺的图片并显示在窗口上
{
    m_strPhoto=t_str;
    QPixmap pixmp(m_strPhoto);
    pixmp=pixmp.scaled(100,100);//将图片缩放适应QGraphicsView大小
    QGraphicsScene *scene = new QGraphicsScene;//实例化一个QGraphicsScene对象
    scene->addPixmap(pixmp);//设置该QGraphicsScene的图片路径，这里采用的是相对路径
    ui->store_photo->setScene(scene);//将QGraphicsScene显示在QGraphicsView控件上
}
store::~store()
{
    delete ui;
    m_dbStore.close();

}
void store::setUsername(QString t_str)//记录顾客用户名
{
    m_strUsername=t_str;
}
void store::on_select_clicked()//选择该店铺的按钮按下触发的槽函数
{
    //进入一个店铺时，更新选餐数据，将临时字段quantity全部置为0，用来实时记录选餐数据
    QSqlQuery query(m_dbStore);
    query.exec(QString("update %1 set quantity = 0 ").arg(m_strName));

    //进入secondwindow_toc页面，即顾客选餐页面
    secondwindow_toc* t_ptrsw=new secondwindow_toc(m_strUsername,this->m_strName,this);
    t_ptrsw->show();
}

