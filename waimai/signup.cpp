#include "signup.h"
#include "ui_signup.h"

#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QLineEdit>
#include <QIntValidator>

//注册界面
Signup::Signup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Signup)
{

    ui->setupUi(this);
    setWindowTitle("注册界面");

    //设置密码输入框的显示模式，提示文字，输入限制为整数
    ui->password->setValidator(new QIntValidator(ui->password));
    ui->password->setPlaceholderText("密码仅支持数字");
    ui->password->setEchoMode(QLineEdit::Password);
    ui->ensure_password->setValidator(new QIntValidator(ui->password));
    ui->ensure_password->setPlaceholderText("密码仅支持数字");
    ui->ensure_password->setEchoMode(QLineEdit::Password);

    //商家设置店名的文本框，并在初始时设置为隐藏
    ui->storename->setPlaceholderText("商家请输入店铺名");
    ui->storename->hide();
    ui->label_4->hide();

    //连接数据库
    m_dbSignup=QSqlDatabase::addDatabase("QSQLITE");
    m_dbSignup.setDatabaseName("waimai.db");
    if(!m_dbSignup.open())
    {
        qDebug()<<"open error: "<<m_dbSignup.lastError();
    }

}

Signup::~Signup()
{
    delete ui;
    m_dbSignup.close();
}

//确认注册按钮按下触发的槽函数
void Signup::on_signup_clicked()
{
    //用户名，密码，确认密码框均有输入时
    if(ui->password->text()==ui->ensure_password->text()&&ui->password->text()!=NULL&&ui->username->text()!=NULL)
    {
        QSqlQuery t_query(m_dbSignup);
        t_query.exec(QString("select * from users where username = '%1'").arg(ui->username->text()));
        if(t_query.next())
        {
            QMessageBox::warning(this,"Warning","该用户名已被注册");
            ui->username->clear();
            ui->password->clear();
            ui->ensure_password->clear();
            ui->username->setFocus();
        }
        else
        {
            //选择注册商家账号
            if(ui->checktype->isChecked())
            {
                //判断是否输入店名，默认在注册时必输入
                if(ui->storename->text()!=NULL)
                {
                    QMessageBox::information(this,NULL,"注册成功");
                    QSqlQuery query(m_dbSignup);//定义数据库操作语句函数
                    //将账号，密码，账号类型记录进users表
                    query.prepare("insert into users (username,password,type) values(:username,:password,:type);");//采用prepare和bindValue方法实现占位符输入变量，后面发现可以使用QString.arg()方法
                    query.bindValue(":username",ui->username->text());//将变量填入上述语句
                    query.bindValue(":password",ui->password->text().toInt());
                    query.bindValue(":type","business");
                    if(!query.exec())//将账号，密码，账号类型记录进users表
                    {
                        qDebug()<<"insert error!"<<m_dbSignup.lastError();
                    }

                    //将用户名，店铺名，店铺地址相关信息记录进store表，用以匹配用户名和店名
                    query.prepare("insert into store (username,storename,address) values(:username,:storename,:address)");
                    query.bindValue(":username",ui->username->text());
                    query.bindValue(":storename",ui->storename->text());
                    query.bindValue(":address","东南大学九龙湖校区");
                    if(!query.exec())//将用户名，店铺名，店铺地址相关信息记录进store表，用以匹配用户名和店名
                    {
                        qDebug()<<"insert error!";
                    }

                    //创建以店铺名为表头的表，用来记录该店中的菜品和销量
                    query.prepare(QString("create table if not exists %1(id INTEGER PRIMARY KEY,name TEXT,price INTEGER,introduction TEXT,photo TEXT DEFAULT './photo/暂无图片.jpg',quantity INTEGER,total INTEGER)").arg(ui->storename->text()));
                    if(!query.exec())
                    {
                        qDebug()<<"create error!"<<m_dbSignup.lastError();
                    }
                    this->close();
                }
                //判断是否输入店名，默认在注册时必输入
                else
                {
                    QMessageBox::warning(this,"Warning","请输入店铺名称");
                    ui->username->clear();
                    ui->password->clear();
                    ui->ensure_password->clear();
                    ui->username->setFocus();
                }
            }
            //选择注册顾客账号
            else
            {
                QMessageBox::information(this,NULL,"注册成功");
                QSqlQuery query(m_dbSignup);
                //将账号，密码记录进users表
                query.prepare("insert into users (username,password,type) values(:username,:password,:type);");
                query.bindValue(":username",ui->username->text());
                query.bindValue(":password",ui->password->text().toInt());
                query.bindValue(":type","customer");

                if(!query.exec())
                {
                    qDebug()<<"insert error!";
                }
                this->close();
            }
        }
    }
    //账号，密码，确认密码输入有误的情况，进行警告
    else if(ui->username->text()==NULL)//未输入账号
    {
        QMessageBox::warning(this,"Warning","请输入账号");
        ui->username->clear();
        ui->password->clear();
        ui->ensure_password->clear();
        ui->username->setFocus();
    }
    else if(ui->password->text()==NULL)//未输入密码
    {
        QMessageBox::warning(this,"Warning","请输入密码");
        ui->username->clear();
        ui->password->clear();
        ui->ensure_password->clear();
        ui->username->setFocus();
    }
    else if(ui->ensure_password->text()==NULL)//未输入确认密码
    {
        QMessageBox::warning(this,"Warning","请确认密码");
        ui->username->clear();
        ui->password->clear();
        ui->ensure_password->clear();
        ui->username->setFocus();
    }
    else if(ui->password->text()!=ui->ensure_password->text())//密码和确认密码输入错误
    {
        QMessageBox::warning(this,"Warning","两次密码输入不一致");
        ui->username->clear();
        ui->password->clear();
        ui->ensure_password->clear();
        ui->username->setFocus();
    }
}

//返回按钮按下后执行的槽函数。关闭注册页面，返回登录界面
void Signup::on_back_clicked()
{
    m_dbSignup.close();
    this->close();
}

//同登录界面的设置密码显示方式，是显式还是隐藏
void Signup::on_password_show_stateChanged(int arg1)
{
    if(arg1==2)
    {
        ui->password->setEchoMode(QLineEdit::Normal);
    }
    else if(arg1==0)
    {
        ui->password->setEchoMode(QLineEdit::Password);
    }
}
void Signup::on_ensure_password_show_stateChanged(int arg1)
{
    if(arg1==2)
    {
        ui->ensure_password->setEchoMode(QLineEdit::Normal);
    }
    else if(arg1==0)
    {
        ui->ensure_password->setEchoMode(QLineEdit::Password);
    }
}


//设置勾选”我是商家“后店铺名输入框的显示和隐藏
void Signup::on_checktype_stateChanged(int arg1)
{
    if(arg1==2)
    {
        ui->label_4->show();
        ui->storename->show();
    }
    else if(arg1==0)
    {
        ui->label_4->hide();
        ui->storename->hide();
    }
}

