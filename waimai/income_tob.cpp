#include "income_tob.h"
#include "ui_income_tob.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QtCharts>

//店铺营收页面
Income::Income(QString t_strStorename, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Income)
{
    ui->setupUi(this);

    //记录店铺名
    setStorename(t_strStorename);

    //打开数据库
    m_dbIncome=QSqlDatabase::addDatabase("QSQLITE");
    m_dbIncome.setDatabaseName("waimai.db");
    if(!m_dbIncome.open())
    {
        qDebug()<<"open error: "<<m_dbIncome.lastError();
    }

    //通过柱状图来展示店铺的营收
    QChartView *chartview=new QChartView(this);//实例化一个QChartView对象（图框）
    ui->m_Layout->addWidget(chartview);//在布局中添加QChartView对象

    QChart *chart=new QChart;//实例化一个QChart对象（图）
    chartview->setChart(chart);//在QChartView中加入QChart对象

    QBarSeries *series = new QBarSeries;//实例化一个QBarSeries对象（柱子集合）


    //从数据库中获取各个菜品卖出的总数量，用于计算营收
    QSqlQuery query(m_dbIncome);
    if(!query.exec(QString("select * from %1 where total != 0").arg(m_strStorename)))
    {
        qDebug()<<"select error!";
    }
    else
    {
        int t_iIncome = 0;
        //将各个菜品的总售出数量*单价 记录到QBarSet中
        while(query.next())
        {
            //设置一个个柱子
            QBarSet *set = new QBarSet(query.value("name").toString());
            int totalprice=query.value("total").toInt()*query.value("price").toInt();
            *set<<totalprice;
            series->append(set);
            t_iIncome+=totalprice;
        }
        ui->lcdNumber->display(t_iIncome);
    }

    chart->addSeries(series);//在QChart中加入QBarSeries对象

    //设置图表的单位长度和坐标轴单位
    chart-> createDefaultAxes();//自动设置图表的单位长度
    chart->axisY()->setTitleText("收入/元");//设置Y轴单位



}

Income::~Income()
{
    delete ui;
    m_dbIncome.close();
}

void Income::setStorename(QString t_str)
{
    m_strStorename=t_str;
}
