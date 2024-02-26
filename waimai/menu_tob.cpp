#include "menu_tob.h"
#include "ui_menu_tob.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQueryModel>

//店铺菜单管理页面
Menu::Menu(QString t_strStorename,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);

    //设置店铺名
    setStoreName(t_strStorename);
    //展示菜单
    refresh();

    //打开数据库
    m_dbMenu=QSqlDatabase::addDatabase("QSQLITE");
    m_dbMenu.setDatabaseName("waimai.db");
    if(!m_dbMenu.open())
    {
        qDebug()<<"open error!";
    }
}

Menu::~Menu()
{
    delete ui;
    m_dbMenu.close();
}

void Menu::refresh()
{
    //利用QSqlQueryModel对表格进行展示，节约了大量单独设置每一列数据来源的代码量
    QSqlQueryModel* model=new QSqlQueryModel(this);
    model->setQuery(QString("select id,name,price,introduction,photo from %1").arg(m_strStoreName));
    ui->tableView->setModel(model);

    //设置表头
    model->setHeaderData(1, Qt::Horizontal, "菜品");
    model->setHeaderData(2, Qt::Horizontal, "单价");
    model->setHeaderData(3, Qt::Horizontal, "介绍");
    model->setHeaderData(4, Qt::Horizontal, "图片地址");
}

//记录店铺名
void Menu::setStoreName(QString t_str)
{
    m_strStoreName=t_str;
    m_dbMenu.close();
}

//添加菜品按钮按下后触发的槽函数
void Menu::on_pushButton_clicked()
{
    //判断是否输入了菜品的名称，单价，介绍（为强制要求）
    if(ui->name->text()!=NULL&&ui->introduction->toPlainText()!=NULL&&ui->price->value()!=0)
    {
        QSqlQuery query(m_dbMenu);
        //判断是否输入了图片（图片为非强制要求）
        if(ui->photo->toPlainText()==NULL)
        {
            //未输入图片信息则只添加(name,price,introduction,quantity,total)信息
            if(!query.exec(QString("insert into %1 (name,price,introduction,quantity,total) values ('%2',%3,'%4',0,0)").arg(m_strStoreName).arg(ui->name->text()).arg(ui->price->value()).arg(ui->introduction->toPlainText())))
            {
                qDebug()<<"insert error!";
            }
        }
        //输入了图片则添加(name,price,introduction,photo,quantity,total)信息
        else
        {
            if(!query.exec(QString("insert into %1 (name,price,introduction,photo,quantity,total) values ('%2',%3,'%4','%5',0,0)").arg(m_strStoreName).arg(ui->name->text()).arg(ui->price->value()).arg(ui->introduction->toPlainText()).arg(ui->photo->toPlainText())))
            {
                qDebug()<<"insert error!";
            }
        }
    }
    //未输入强制要求的信息，弹出警告
    else
    {
        QMessageBox::warning(this,"warning","请完整输入要添加的菜品的名称、单价、和介绍",QMessageBox::Ok);
    }
    //菜单刷新
    refresh();

}

//按下刷新按钮后触发的槽函数（应对bug）
void Menu::on_pushButton_2_clicked()
{
    refresh();
}

//按下删除按钮后触发的槽函数
void Menu::on_pushButton_3_clicked()
{
    //删除菜品以id为索引
    QSqlQuery query(m_dbMenu);
    //判断是否输入id号
    if(ui->id->text()==NULL)
    {
        //未输入id号，弹出警告
        QMessageBox::warning(this,"warning","请输入要删除的菜品id",QMessageBox::Ok);
        ui->id->setFocus();//移动光标至id号
    }
    //输入了id号
    else
    {
        //利用select count(*)语句判断id号是否存在
        query.exec(QString("select count(*) from %1 where id= %2").arg(m_strStoreName).arg(ui->id->text().toInt()));
        query.next();
        //id号不存在时，弹出警告
        if(query.value(0).toInt()==0)
        {
            QMessageBox::warning(this,"warning","查无此id",QMessageBox::Ok);
            ui->id->clear();
            ui->id->setFocus();
        }
        //id号存在时，将对应id号的菜品删除
        else
        {
            query.exec(QString("delete from %1 where id= %2").arg(m_strStoreName).arg(ui->id->text().toInt()));
            refresh();
        }

    }
}


//修改按钮按下后触发的槽函数
void Menu::on_pushButton_4_clicked()
{
    //修改菜品以id号为索引
    QSqlQuery query(m_dbMenu);
    //判断是否输入id号
    if(ui->id->text()==NULL)//未输入id号
    {
        QMessageBox::warning(this,"warning","请输入要修改的菜品id",QMessageBox::Ok);
        ui->id->setFocus();
    }
    else
    {
        //判断id号是否存在
        query.exec(QString("select count(*) from %1 where id= %2").arg(m_strStoreName).arg(ui->id->text().toInt()));
        query.next();
        if(query.value(0).toInt()==0)//id号不存在
        {
            QMessageBox::warning(this,"warning","查无此id",QMessageBox::Ok);
            ui->id->clear();
            ui->id->setFocus();
        }
        //id号存在，进行修改。因为修改时不确定哪些需要修改，对每个修改加了一个条件判断，如果修改框内有文字，则修改
        else
        {
            if(ui->name->text()!=NULL)
            {
                query.exec(QString("update %1 set name='%2' where id = %3").arg(m_strStoreName).arg(ui->name->text()).arg(ui->id->text().toInt()));
            }
            if(ui->price->text().toInt()!=0)
            {
                query.exec(QString("update %1 set price=%2 where id = %3").arg(m_strStoreName).arg(ui->price->value()).arg(ui->id->text().toInt()));
            }
            if(ui->introduction->toPlainText()!=NULL)
            {
                query.exec(QString("update %1 set introduction='%2' where id = %3").arg(m_strStoreName).arg(ui->introduction->toPlainText()).arg(ui->id->text().toInt()));
            }
            if(ui->photo->toPlainText()!=NULL)
            {
                query.exec(QString("update %1 set photo ='%2' where id = %3").arg(m_strStoreName).arg(ui->photo->toPlainText()).arg(ui->id->text().toInt()));
            }
            //刷新显示
            refresh();
        }
    }
}

