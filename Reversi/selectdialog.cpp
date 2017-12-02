#include "selectDialog.h"
#include<QSpacerItem>

#include"base.h"
#include"aimode.h"
#include"netmode.h"

selectDialog ::selectDialog (QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    s = new stacked;
    btn1 = new QPushButton("one");

    //设置按钮图片,无动作时,鼠标移动到其上时,点击时
    #if 1
    btn1->setStyleSheet(
                "QPushButton{background-image: url(:/new/prefix1/images/1-1.png);}"
    "QPushButton:hover{background-image: url(:/new/prefix1/images/2-1.png);}"
    "QPushButton:pressed{background-image: url(:/new/prefix1/images/3-1.png);}");
    #endif
    btn2 = new QPushButton("开始");
    connect(btn2,SIGNAL(clicked(bool)),this,SLOT(slotSelect()));
    btn3 = new QPushButton("退出");
    connect(btn3,SIGNAL(clicked(bool)),this,SLOT(accept()));
    btn4 = new QPushButton("four");
    QHBoxLayout *hLay = new QHBoxLayout();
    //QSpacerItem *spacer = new QSpacerItem(400,200);
    //hLay->addItem(spacer);
        hLay->addStretch(1);
    hLay->addStretch(1);
 //   hLay->addWidget(btn1);
    hLay->addWidget(btn2);
        hLay->addStretch(1);
    hLay->addWidget(btn3);
    hLay->addStretch(1);
        hLay->addStretch(1);
 //   hLay->addWidget(btn4);

    QVBoxLayout *vLay = new QVBoxLayout(this);
    QLabel *tishi = new QLabel("请选择模式");
        tishi->setStyleSheet("QLabel{font:bold 32px; color:blue}");
    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->addStretch(1);
    hlay2->addWidget(tishi);
    hlay2->addStretch(1);

    vLay->addLayout(hlay2);
    vLay->addWidget(s,3);
    vLay->addLayout(hLay,1);
    //填充背景图片
    _image.load(":/new/prefix1/22.jpg");
    setAutoFillBackground(true);
}

selectDialog::~selectDialog()
{
}

//重写函数,实现图片拉伸填充窗口
void selectDialog::resizeEvent(QResizeEvent *event)
{
    //调用父类的resizeEvent,处理
    QWidget::resizeEvent(event);
    QPalette pal(palette());

// 注意下面这一行，就是靠他实现大小拉伸～～～
    pal.setBrush(QPalette::Window,
                QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,
                        Qt::SmoothTransformation)));
    setPalette(pal);
}

void selectDialog::slotSelect()
{
   if((s->leftlist->currentRow()) == 0)
   {
         mytest *m = new mytest;
         m->show();
   }
   else
   {
       if(s->leftlist->currentRow() == 1)
       {
            bool ahead;
            int level;
            //ahead为false时是执黑,level为难度
            s->a->setting(ahead,level);
            AiMode *ai = new AiMode(ahead,level);
            ai->show();
       }
       else
       {
           if(s->leftlist->currentRow() == 2)
           {
                bool isServer;
                bool color;
                s->n->setting(isServer,color);
                NetMode * net = new NetMode(isServer,color);
                net->show();
           }
       }
   }
   accept();
}
