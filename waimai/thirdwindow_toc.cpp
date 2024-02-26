#include "thirdwindow_toc.h"
#include "ui_thirdwindow_toc.h"

#include <QTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

//顾客的第三个页面，即订单界面
thirdwindow_toc::thirdwindow_toc(QString t_strUsername,QString t_strStorename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::thirdwindow_toc)
{
    ui->setupUi(this);

    //将菜单栏返回按钮与本页面关闭关联
    connect(ui->menu,SIGNAL(aboutToShow()),this,SLOT(close()));

    //记录店铺名称顾客用户名
    setStoreName(t_strStorename);
    setUsername(t_strUsername);

    //连接数据库
    m_dbbill=QSqlDatabase::addDatabase("QSQLITE");
    m_dbbill.setDatabaseName("waimai.db");
    if(!m_dbbill.open())
    {
        qDebug()<<"open error!"<<m_dbbill.lastError();
    }

    QGraphicsScene *scene = new QGraphicsScene;//实例化一个QGraphicsScene对象
    scene->addPixmap(QPixmap("./photo/QRcode1.png"));//设置该QGraphicsScene的图片路径，这里采用的是相对路径
    ui->QRcode->setScene(scene);//将图片显示在QRcode控件上

    ui->bill->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置账单表格不可编辑

    //获取数据库中菜品数据来构建菜单
    QSqlQuery query(m_dbbill);
    QString t=m_strStoreName;
    if(!query.exec(QString("select name,price,quantity from %1 where quantity>0").arg(m_strStoreName)))
    {
        qDebug()<<"select error!"<<t<<m_dbbill.lastError();
    }
    //将数据填进tablewidget控件中，该控件可显示一个表格
    else
    {
        while(query.next())
        {
            int t_iRowx=ui->bill->rowCount();//获取目前有多少行
            ui->bill->insertRow(t_iRowx);//在改行后面添加一行
            //new 几个表格item填入数据
            QTableWidgetItem* itemName(new QTableWidgetItem(query.value("name").toString()));//菜品名称
            QTableWidgetItem* itemPrice(new QTableWidgetItem(query.value("price").toString()));//单价
            QTableWidgetItem* itemQuantity(new QTableWidgetItem(query.value("quantity").toString()));//数量
            QTableWidgetItem* itemTotal(new QTableWidgetItem(QString::number(query.value("price").toInt()*query.value("quantity").toInt())));//单个菜品的总价
            //void QTableWidget::setItem(int row, int column, QTableWidgetItem *item)，将item填入表格行列中，该坐标从左上角（0,0）开始
            ui->bill->setItem(t_iRowx,0,itemName);
            ui->bill->setItem(t_iRowx,1,itemPrice);
            ui->bill->setItem(t_iRowx,2,itemQuantity);
            ui->bill->setItem(t_iRowx,3,itemTotal);
        }
    }

}

thirdwindow_toc::~thirdwindow_toc()
{
    delete ui;
    m_dbbill.close();
}

//在最后加上配送费所有菜品的总价之和
void thirdwindow_toc::setPayment(int t_i)
{
    //添加一行用来展示配送费
    m_iPayment=t_i;
    int t_iRowx=ui->bill->rowCount();
    ui->bill->insertRow(t_iRowx);
    ui->bill->setItem(t_iRowx,2,new QTableWidgetItem ("配送费："));
    //配送费的具体费送采用随机数生成，为兼容低版本，采用qsrand和qrand
    qsrand(QTime::currentTime().msec());
    double t_iTip=(qrand()%30+20)*0.1;
    ui->bill->setItem(t_iRowx,3,new QTableWidgetItem(QString::number(t_iTip)));

    //添加一行来表示所有菜品的总价之和
    t_iRowx=ui->bill->rowCount();
    ui->bill->insertRow(t_iRowx);
    ui->bill->setItem(t_iRowx,2,new QTableWidgetItem("合计："));
    //总价之和包括菜品总价+配送费，为实际应付款
    ui->bill->setItem(t_iRowx,3,new QTableWidgetItem(QString::number(m_iPayment+t_iTip)));

}

//记录店铺名称和顾客用户名
void thirdwindow_toc::setStoreName(QString t_str)
{
    m_strStoreName=t_str;
    qDebug()<<m_strStoreName;
}
void thirdwindow_toc::setUsername(QString t_str)
{
    m_strUsername=t_str;
    qDebug()<<m_strUsername;
}

//将订单记录进数据库，这时店铺名和顾客用户名也同时被记录
void thirdwindow_toc::on_pushButton_clicked()
{
    QSqlQuery query(m_dbbill);

    //在店铺表中找到该顾客的字段，若找不到则生成一个，用来记录该顾客的订单
    query.prepare("PRAGMA table_info(" + m_strStoreName + ")");
    if (query.exec())
    {
        bool fieldExists = false;
        while (query.next())// 执行查询，检查字段是否存在
        {
            QString columnName = query.value("name").toString();
            if (columnName == m_strUsername)
            {
                fieldExists = true;
                break;
            }
        }
        if(!fieldExists)//若不存在则创建一个
        {
            query.exec(QString("alter table %1 add column %2 INTEGER DEFAULT 0").arg(m_strStoreName).arg(m_strUsername));
        }
    }
    query.exec(QString("update %1 set %2=quantity+%2").arg(m_strStoreName).arg(m_strUsername));//更新该用户点的订单
    query.exec(QString("update %1 set quantity=0").arg(m_strStoreName));//将临时订单重置为0
    QMessageBox::information(this,"订单支付成功","即将返回主页面");
    this->close();
    emit closeSecondwindow();//发送关闭选餐窗口的信号

}



