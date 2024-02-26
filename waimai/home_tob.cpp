#include "home_tob.h"
#include "ui_home_tob.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsPixmapItem>

//店铺的个人中心页面
home::home(QString t_strUsername, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::home)
{
    ui->setupUi(this);

    //记录用户名
    setUsername(t_strUsername);

    //打开数据库
    m_dbhome=QSqlDatabase::addDatabase("QSQLITE");
    m_dbhome.setDatabaseName("waimai.db");
    if(!m_dbhome.open())
    {
        qDebug()<<"open error!";
    }

    //展示个人中心页面的信息
    refresh();

}

home::~home()
{
    delete ui;
    m_dbhome.close();
}

//记录用户名的函数
void home::setUsername(QString t_str)
{
    m_strUsername=t_str;
}
//记录店铺名的函数
void home::setStorename(QString t_str)
{
    m_strStorename=t_str;
}

//修改个人信息的按钮按下后触发的槽函数
void home::on_change_clicked()
{
    //判断是否在修改框中输入了店铺名和地址，这是最基本信息
    if(ui->storename->toPlainText()==NULL||ui->storeaddress->toPlainText()==NULL)
    {
        QMessageBox::warning(this,"修改失败","店铺名称和地址不能为空！",QMessageBox::Ok);
    }
    //将信息更新进store数据库中
    else
    {
        QSqlQuery t_query(m_dbhome);
        t_query.exec(QString("update store set storename = '%1',address = '%2' where username = '%3'").arg(ui->storename->toPlainText()).arg(ui->storeaddress->toPlainText()).arg(m_strUsername));
        //判断是否需要更新店铺图片
        if(ui->storephoto->toPlainText()!=NULL)
        {
            t_query.exec(QString("update store set photo = '%1' where username = '%2'").arg(ui->storephoto->toPlainText()).arg(m_strUsername));
        }
        //因为修改了店铺名，需要将以店铺名为表名的表更名
        t_query.exec(QString("alter table %1 rename to %2").arg(m_strStorename).arg(ui->storename->toPlainText()));
        //刷新显示，显示修改后的数据
        refresh();
        //修改重要数据后需要返回登录界面重新登录
        QMessageBox::information(this,NULL,"修改成功，即将返回登录界面",QMessageBox::Ok);
        //发送关闭店铺主页面的信号
        emit closewindow();
        //关闭本页面
        this->close();
    }
}

//显示函数
void home::refresh()
{
    //读取数据库的相关数据
    QSqlQuery query(m_dbhome);
    if(!query.exec(QString("select storename,address,photo from store where username = '%1' ").arg(m_strUsername)))
    {
        qDebug()<<"select error!"<<m_strUsername;
    }
    //将数据库中的数据显示在控件上
    else
    {
        query.next();
        //将数据显示出来
        ui->storename->setText(query.value("storename").toString());//显示店铺名
        ui->storeaddress->setText(query.value("address").toString());//显示店铺地址
        ui->storephoto->setText(query.value("photo").toString());//显示图片“地址”
        setStorename(query.value("storename").toString());//记录该用户名对应的店铺名，用于后续修改时找到这个表
        //显示图片
        QPixmap pixmp(query.value("photo").toString());
        pixmp=pixmp.scaled(180,180);//将图片缩放适应QGraphicsView大小
        QGraphicsScene *scene = new QGraphicsScene;//实例化一个QGraphicsScene对象
        scene->addPixmap(pixmp);//设置该QGraphicsScene的图片路径，这里采用的是相对路径
        ui->photo->setScene(scene);//将QGraphicsScene显示在QGraphicsView控件上
    }
}

