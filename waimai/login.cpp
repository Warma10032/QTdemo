#include "login.h"
#include "ui_login.h"
#include "mainwindow_toc.h"
#include "mainwindow_tob.h"
#include "signup.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QIntValidator>

//登录界面
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowTitle("登录界面");

    //设置password输入框只能输入数字
    ui->password->setValidator(new QIntValidator(ui->password));
    ui->password->setPlaceholderText("密码仅支持数字");//设置提示
    ui->password->setEchoMode(QLineEdit::Password);//设置输入的显示方式，echomode=password，输入的密码会变为黑点隐藏

    //连接数据库
    m_dbLogin=QSqlDatabase::addDatabase("QSQLITE");
    m_dbLogin.setDatabaseName("waimai.db");
    if(!m_dbLogin.open())
    {
        qDebug()<<"open error: "<<m_dbLogin.lastError();
    }
}

Login::~Login()
{
    delete ui;
    m_dbLogin.close();
}

//按下登录按钮后触发的槽函数
void Login::on_login_clicked()
{
    QSqlQuery query(m_dbLogin);
    query.prepare("select username,password,type from users where username=:username");
    query.bindValue(":username",ui->username->text());
    query.exec();//定位到数据集第一条数据的前一个数据位置
    bool t_bExisted=query.first();//返回的数据集停留在第一条数据之前,执行该语句跳转到第一条，若没有结果第一条为空则返回false//后来发现查找数据库数据是否存在可以用SELECT count(object) FROM tablename...，会返回object的数目
    //用户名不存在时
    if(!t_bExisted)
    {
        QMessageBox::warning(this,"登录失败","用户名不存在！",QMessageBox::Ok);//弹出提示消息框
        ui->username->clear();//清空输入文本框
        ui->password->clear();
        ui->username->setFocus();//将光标重置于用户名输入框
    }
    //用户名存在且密码正确时
    else if(query.value("password")==ui->password->text().toInt()){
        this->close();
        if(query.value("type")=="customer")//判断用户是否为顾客
        {
            MainWindow_toc *t_ptrMainWindow=new MainWindow_toc(ui->username->text(),this);//实例化顾客主页面
            t_ptrMainWindow->show();
        }
        else if(query.value("type")=="business")//判断用户是否为商家
        {
            MainWindow_tob *t_ptrMainWindow=new MainWindow_tob(ui->username->text(),this);//实例化商家主界面
            t_ptrMainWindow->show();
        }

    }
    //用户名存在但密码错误
    else if(query.value("password")!=ui->password->text().toInt()){
        QMessageBox::warning(this,"登录失败","用户名或密码输入错误！",QMessageBox::Ok);//弹出提示消息框
        ui->username->clear();
        ui->password->clear();
        ui->username->setFocus();
    }
}

//注册按钮被按时触发的槽函数
void Login::on_signup_clicked()
{
    Signup *s=new Signup;//实例化一个注册页面
    s->show();
}

//用来调整密码的显示还是隐藏
void Login::on_password_show_stateChanged(int arg1)
{
    if(arg1==2)//==2为选中该框，因为默认==1为半选中
    {
        ui->password->setEchoMode(QLineEdit::Normal);
    }
    else if(arg1==0)//==0为未选中
    {
        ui->password->setEchoMode(QLineEdit::Password);
    }
}

