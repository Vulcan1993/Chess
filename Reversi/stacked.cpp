#include "stacked.h"
stacked::stacked(QWidget *parent) : QWidget(parent)
{
    //setWindowTitle("StackedWidget");
    //设置列表框
    m_leftlist = new QListWidget(this);

    //设置无边框
    m_leftlist->setFrameShape(QListWidget::NoFrame);
    //设置图片背景为透明
 //   leftlist->setStyleSheet("background-color:transparent");
    //设置字体
    m_leftlist->setStyleSheet("QListWidget{font:bold 17px; color:red ; background-color:transparent}");
    //加行
    m_leftlist->insertItem(0,"人人对战");
    m_leftlist->insertItem(1,"人机对战");
    m_leftlist->insertItem(2,"网络对战");


    //设置默认选择的项
    m_leftlist->setCurrentRow(0);

    //设置堆栈窗体
     m_multiPage = new MultiPage(this);
     m_aiPage = new AiPage(this);
     m_netPage = new NetPage(this);
    //创建堆栈窗体
    m_stack = new QStackedWidget(this);
    //加页面
    m_stack->addWidget(m_multiPage);
    m_stack->addWidget(m_aiPage);
    m_stack->addWidget(m_netPage);

    //设置主窗体布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    //边距设为5
    mainLayout->setMargin(5);
    //内部控件间距设为5
    mainLayout->setSpacing(5);
    mainLayout->addWidget(m_leftlist);
    mainLayout->addWidget(m_stack,0,Qt::AlignHCenter);
    //设置leftlist和stack占窗口空间的比例是1:3
    mainLayout->setStretchFactor(m_leftlist,1);
    mainLayout->setStretchFactor(m_stack,3);
    //连接信号和槽,实现选择leftlist的第几行就显示第几个label
    connect(m_leftlist,SIGNAL(currentRowChanged(int)),m_stack,SLOT(setCurrentIndex(int)));

}
