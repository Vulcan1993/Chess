#include"compconfig.h"
#include "stacked.h"
stacked::stacked(QWidget *parent) : QWidget(parent)
{
    //setWindowTitle("StackedWidget");
    //设置列表框
    leftlist = new QListWidget(this);

    //设置无边框
    leftlist->setFrameShape(QListWidget::NoFrame);
    //设置图片背景为透明
 //   leftlist->setStyleSheet("background-color:transparent");
    //设置字体
    leftlist->setStyleSheet("QListWidget{font:bold 17px; color:red ; background-color:transparent}");
    //加行
    leftlist->insertItem(0,"人人对战");
    leftlist->insertItem(1,"人机对战");
    leftlist->insertItem(2,"网络对战");



    //设置默认选择的项
    leftlist->setCurrentRow(0);

    //设置堆栈窗体
     m = new MultiPage(this);
     a = new AiPage(this);
     n = new NetPage(this);
    //创建堆栈窗体
    stack = new QStackedWidget(this);
    //加页面
    stack->addWidget(m);
    stack->addWidget(a);
    stack->addWidget(n);

    //设置主窗体布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    //边距设为5
    mainLayout->setMargin(5);
    //内部控件间距设为5
    mainLayout->setSpacing(5);
    mainLayout->addWidget(leftlist);
    mainLayout->addWidget(stack,0,Qt::AlignHCenter);
    //设置leftlist和stack占窗口空间的比例是1:3
    mainLayout->setStretchFactor(leftlist,1);
    mainLayout->setStretchFactor(stack,3);
    //连接信号和槽,实现选择leftlist的第几行就显示第几个label
    connect(leftlist,SIGNAL(currentRowChanged(int)),stack,SLOT(setCurrentIndex(int)));

}

