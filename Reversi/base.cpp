#include"base.h"
#include<QDebug>
mytest::mytest(QWidget *parent)
    :QWidget(parent)
{   //窗口控件布局
    m_btn1=new QPushButton("开始",this);
    m_btn2=new QPushButton("悔棋",this);
    m_btn3=new QPushButton("保存",this);
    m_btn4=new QPushButton("读取",this);

    m_btn5=new QPushButton("返回",this);

    m_btn6=new QPushButton("一步",this);
    m_btn7=new QPushButton("",this);
    m_btn8=new QPushButton("提示可下",this);
    m_btn9=new QPushButton("重新开始",this);

    //用label显示状态
    m_label = new QLabel("就绪");
    m_label2 = new QLabel;
    //设置QLabel风格
    m_label->setStyleSheet("QLabel{font:bold 16px; color:red}");
    m_label2->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    m_spacer= new QSpacerItem(640,640);
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addItem(m_spacer);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(m_label);
    hlay->addWidget(m_label2);
    vlay->addLayout(hlay);

    m_vLayout= new QVBoxLayout;
    m_vLayout->addWidget(m_btn1);
    m_vLayout->addWidget(m_btn2);
    m_vLayout->addWidget(m_btn3);
    m_vLayout->addWidget(m_btn4);
    m_vLayout->addWidget(m_btn5);
    m_vLayout->addWidget(m_btn6);
    m_vLayout->addWidget(m_btn7);
    m_vLayout->addWidget(m_btn8);
    m_vLayout->addWidget(m_btn9);

    m_vLayout->addStretch(1);

    m_hLayout =new QHBoxLayout;
    //  hLayout->addItem(spacer);
    m_hLayout->addLayout(vlay);
    m_hLayout->addLayout(m_vLayout);

    setLayout(m_hLayout);

    m_timer= new QTimer;
    m_timer->setInterval(500);
    // connect(timer,SIGNAL(timeout()),this,SLOT(paint()));

    m_timer->start();

    //初始化棋盘
    int i,j;
    for(i=0 ; i<8 ; ++i)
    {
        for( j=0 ; j<8 ; ++j)
        {
            //强制类型转换，把int转换为mytest::type类型(实际值是相同的)
            m_board[i][j] =static_cast<mytest::type>(0);
            // board[i][j] =static_cast<mytest::type>(qrand()%3);
        }
    }

    m_board[3][3]=white;m_board[4][4]=white;
    m_board[3][4]=black;m_board[4][3]=black;




    //connect(btn1,SIGNAL(clicked()),this,SIGNAL(signal1()));
    //connect(this,SIGNAL(signal1()),this,SLOT(one()));

    //connect(btn1,SIGNAL(clicked(bool)),this,SLOT(one()));//与上面的两行功能相同

 //   connect(btn2,SIGNAL(clicked(bool)),btn1,SIGNAL(clicked(bool)));// 让btn2与btn1功能相同
   // connect(btn3,SIGNAL(clicked(bool)),this,SLOT(two()));
    //connect(btn5,SIGNAL(clicked(bool)),this,SLOT(slot3_3()));
    //connect(btn6,SIGNAL(clicked(bool)),this,SLOT(slot1_5()));
    //connect(btn7,SIGNAL(clicked(bool)),this,SLOT(slot5_1()));

    connect(m_btn3,SIGNAL(clicked(bool)),this,SLOT(slot_write()));
    connect(m_btn4,SIGNAL(clicked(bool)),this,SLOT(slot_read()));
    connect(m_btn5,SIGNAL(clicked(bool)),this,SLOT(slot_goBack()));
    connect(m_btn8,SIGNAL(clicked(bool)),this,SLOT(slot_earning()));
    connect(m_btn9,SIGNAL(clicked(bool)),this,SLOT(slot_restart()));
    connect(m_btn2,SIGNAL(clicked()),this,SLOT(slot_retract()));
    connect(m_btn5,SIGNAL(clicked(bool)),this,SLOT(slotDebug()));

    //初始化方向数组
    m_direction[0]=QPoint(-1,0);		//上
    m_direction[1]=QPoint(1,0);		//下
    m_direction[2]=QPoint(0,-1);		//左
    m_direction[3]=QPoint(0,1);		//右
    m_direction[4]=QPoint(-1,-1);		//左上
    m_direction[5]=QPoint(1,-1);		//左下
    m_direction[6]=QPoint(-1,1);		//右上
    m_direction[7]=QPoint(1,1);		//右下
 //earning(true);
 m_label->setText("当前状态:黑子下 ");

}

void mytest::paintEvent(QPaintEvent *ev)
{
    //在pixmap里绘制好一次性画到窗口
    QPixmap pixmap(640,640);
 //   pixmap.fill(Qt::gray);

    //在pixmap上画图
    QPainter painter(&pixmap);
    //设置画笔
    QPen pen;
    QImage image;
#if 1
    image.load(":/new/prefix1/board");
    //画刷,(用图片填充);
    QBrush brush;
    brush.setTextureImage(image);
    painter.setBrush(brush);
    painter.drawRect(0,0,640,640);
#endif
#if 0
    //画棋盘
    pen.setColor(Qt::blue);
    pen.setWidth(4);
    painter.setPen(pen);
    //画11条横线
    for(int i=0;i<9;i++)
    {
        painter.drawLine(0,i*d,640,i*d);
    }
    //画11条竖线
    for(int i=0;i<9;i++)
    {
        painter.drawLine(i*d,0,i*d,640);
    }
#endif


    //画棋子
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
            switch(m_board[i][j])
            {
            case black:
                pen.setColor(Qt::black);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.setBrush(QBrush(Qt::black));
                painter.drawEllipse(j*m_step+5,i*m_step+5,70,70);
                break;
            case white:
                pen.setColor(Qt::white);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.setBrush(QBrush(Qt::white));
                painter.drawEllipse(j*m_step+5,i*m_step+5,70,70);
                break;
            case whitecircle:
                pen.setColor(Qt::white);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.setBrush(QBrush(QColor(0,0,0,0)));
                painter.drawEllipse(j*m_step+5,i*m_step+5,70,70);
                break;
            case blackcircle:
                pen.setColor(Qt::black);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.setBrush(QBrush(QColor(0,0,0,0)));
                painter.drawEllipse(j*m_step+5,i*m_step+5,70,70);
                break;
            case kong:
                break;
            }
    }

    //结束绘图
    painter.end();
    //在主窗口上画图
    painter.begin(this);
    //在窗口的(0,0)位置画pixmap
    painter.drawPixmap(0,0,pixmap);
}

void mytest::clearEarning()
{
    int i,j;
    for(i=0 ; i<8 ; ++i)
    {
        for(j=0 ; j<8 ; ++j)
        {
            if(m_board[i][j] == whitecircle || m_board[i][j] == blackcircle)
                m_board[i][j] = kong;
        }
    }
}

int mytest::judge()
{
    int i,j,sum1=0,sum2=0;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            if(m_board[i][j]==white)
                sum1++;
            if(m_board[i][j]==black)
                sum2++;

        }
    }
    m_label->setText(QString("游戏结束：黑方%1个子，白子%2个子").arg(sum2).arg(sum1));
    //arg()用字符串变量参数依次替代字符串中最小数值
    if(sum1>sum2)
    {
        return 1;

     }
    else
    {
        if(sum1<sum2)
        {
            return 0;
        }
        else
            return -1;
    }
}



void mytest::mousePressEvent(QMouseEvent *ev)
{
    if(m_winner)
        return;
    int i,j;
    j=ev->pos().x()/m_step;
    i=ev->pos().y()/m_step;
    QPoint p(i,j);
    //不在棋盘内
    if(i<0||i>8||j<0||j>8)
        return;


    if(ev->button()==Qt::LeftButton)
    {
        vector<QPoint> v;
        onepointexec(v,p,m_b);
        if(0 != v.size())
        {
            //存储这一步
            record(v,p,m_b);
            if(m_b==true)
                m_board[i][j]=white;
            else
                m_board[i][j]=black;
            for(QPoint a:v)
            {
                if(m_b==true)
                    m_board[a.x()][a.y()]=white;
                else
                    m_board[a.x()][a.y()]=black;
            }
            //该另一方走了
            //清除之前的提示
            clearEarning();
            result();
        }
    }//leftbutton

    if(ev->button()==Qt::RightButton)
        m_board[i][j]=kong;
    update();
}









void mytest::slot_earning()
{
    earning();
    update();
}

void mytest::slot_restart()
{
    m_winner=false;
    m_retVB.clear();
    m_retVP.clear();
    m_retVV.clear();
    //初始化棋盘
    int i,j;
    for(i=0 ; i<8 ; ++i)
    {
        for( j=0 ; j<8 ; ++j)
        {
            //强制类型转换，把int转换为mytest::type类型(实际值是相同的)
            m_board[i][j] =static_cast<mytest::type>(0);
            // board[i][j] =static_cast<mytest::type>(qrand()%3);
        }
    }
    m_b=false;
    m_label->setText("当前状态: 黑子下");
    m_board[3][3]=white;m_board[4][4]=white;
    m_board[3][4]=black;m_board[4][3]=black;
    update();
}

void mytest::slot_write()
{
    if(write() == true)
        m_label->setText("保存成功");
    else
        m_label->setText("保存失败");
}

void mytest::slot_read()
{
    QString str;
    if(read() == true)
        str="读取成功";
    else
    {
        str ="读取失败";
        return;
    }
    if(m_b == true)
        str += " : 白方下";
    else
        str += " : 黑方下";
    m_label->setText(str);
    update();
}

void mytest::slot_goBack()
{
    close();
    selectDialog *select = new selectDialog();
    select->show();
}

void mytest::earn(vector<QPoint> &vp,vector<vector<QPoint>> &vv,bool side ,bool state)
{
    QPoint p;
    vector<QPoint> v;
    int i,j;
    for(i=0 ; i<8 ; ++i)
    {
        for(j=0 ; j<8 ; ++j)
        {
            v.clear();
            onepointexec(v,QPoint(i,j),side);
            if(!v.empty())
            {
#if debug_one
                qDebug()<<i<<","<<j<<"**************";
                for(auto a :v)
                    qDebug()<<a;
#endif
                vp.push_back(QPoint(i,j));
                if(state==false)
                    vv.push_back(v);
            }
        }
    }
}


int mytest::earning()
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;

    earn(vp,vv,m_b);
    for(QPoint a:vp)
    {
        if(m_b==true)
            m_board[a.x()][a.y()]=whitecircle;
        else
            m_board[a.x()][a.y()]=blackcircle;
    }
    return vp.size();
#if debug_two
    qDebug()<<vp.size()<< "    "<<vv.size();

    for(int i=0 ; i<vp.size() ; ++i)
    {
        qDebug()<<"i="<<i<<"   ****************";
        qDebug()<<"vp["<<i<<"]="<<vp[i];
        for(QPoint p : vv[i])
            qDebug()<<p;
    }
    qDebug()<<vp.size();
    vp.clear();
    qDebug()<<vp.size();
    for(QPoint p:vp)
        qDebug()<<p;
    qDebug()<<vp.size();
#endif
}

int mytest::result()
{
    //b取反，另一方走
    int i=0;
    m_b = !m_b;
    if(m_b == true)
        m_label->setText("当前状态: 白子下");
    else
        m_label->setText("当前状态: 黑子下");

    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,m_b);
    if(vp.empty())
    {
        if(m_b == true)
            {
                m_label->setText("当前状态:白子轮空,黑子下");
                i = 1;
            }
        else
            {
                m_label->setText("当前状态: 黑子轮空,白子下");
                i = 1;
            }
        vp.clear();
        //b取反，判断另一方是否能走
        m_b = !m_b;
        earn(vp,vv,m_b);
        if(vp.empty())
        {
            ////两者皆空,结束,则判断
            int r=judge();
            m_winner=true;
            if(r==1)
                QMessageBox::information(this,
                                         tr("游戏结果"),
                                         tr("白方胜!")
                                         );
            if(r==0)
                QMessageBox::information(this,
                                         tr("游戏结果"),
                                         tr("黑方胜!")
                                         );
            if(r==-1)
                QMessageBox::information(this,
                                         tr("游戏结果"),
                                         tr("平局!")
                                         );

        }
    }
    return i;
}

void mytest::retract()
{
    if(m_retVP.empty())
        return;
    int i=m_retVP.size()-1;
    m_b = m_retVB[i];
    m_board[m_retVP[i].x()][m_retVP[i].y()] = kong;
    for(QPoint p : m_retVV[i])
    {
        if(m_retVB[i] == true)
            m_board[p.x()][p.y()] = black;
        else
            m_board[p.x()][p.y()] = white;
    }
    m_retVB.pop_back();
    m_retVV.pop_back();
    m_retVP.pop_back();
    if(m_b == true)
        m_label->setText("当前状态: 白子下");
    else
        m_label->setText(("当前状态: 黑子下"));

}

void mytest::record(vector<QPoint> &v, QPoint p, bool side)
{
    m_retVV.push_back(v);
    m_retVP.push_back(p);
    m_retVB.push_back(side);
}

//如果该点能下,则v里边保存所有能翻的点
void mytest::onepointexec(vector<QPoint> &v,QPoint p,bool side)
{
    int i = p.x();
    int j = p.y();
    type color;
    type colorDuiFang;
    //不在棋盘内
    if(m_board[i][j] != white && m_board[i][j] != black)
    {
        if(side==true)
        {
            m_board[i][j]=white;
            color = white;
            colorDuiFang = black;
        }
        if(side!=true)
        {
            m_board[i][j]=black;
            color = black;
            colorDuiFang = white;
        }
    }
    else
    {
        return;
    }
    int fx;
    //依次查找每个方向
    for( fx= 0 ; fx < 8 ; ++fx)
    {
        int x,y;
        x = i + m_direction[fx].x();
        y = j + m_direction[fx].y();
        while(true)
        {
            //如果越界,则退出
            if(x <0 || x>7 || y<0 || y>7)
                break;
            //找到本方向同色的棋子即退出
            if(m_board[x][y] == color)
                break;
            if(m_board[x][y] == kong || m_board[x][y] == whitecircle || m_board[x][y] == blackcircle)
                break;
            //检测下一个点
            x = x + m_direction[fx].x();
            y = y + m_direction[fx].y();
        }
        //如果没越界则判断
        if(!(x <0 || x>7 || y<0 || y>7))
        {
            //如果是因为找到同色的棋子而推出的
            if(m_board[x][y] == color)
            {
                //检测是否相邻,如果不相邻,则(x,y)和(i,j)即为边界点
                if(x != i+m_direction[fx].x() || y != j+m_direction[fx].y())
                {
                    int t1,t2;
                    t1 = i+m_direction[fx].x();
                    t2 = j+m_direction[fx].y();
                    while(t1 != x || t2 != y)
                    {
                        v.push_back(QPoint(t1,t2));
                        t1 = t1 + m_direction[fx].x();
                        t2 = t2 + m_direction[fx].y();
                    }
                }
            }
        }
    }
    m_board[i][j]=kong;
}

bool mytest::write()
{
    QFile file(m_fileName);
    file.open(QIODevice::ReadWrite);
    QDataStream out(&file);
    //保存棋盘
    int i,j;
    for(i=0 ; i<8 ; ++i)
    {
        for(j=0 ; j<8 ; ++j)
        {
            out<<m_board[i][j];
        }
    }
    //保存该谁走了
    out<<m_b;
    file.close();
    return true;
}

bool mytest::read()
{
    QFile file(m_fileName);
    file.open(QIODevice::ReadWrite);
    QDataStream in(&file);
    //保存棋盘
    int i,j;
    int num;
    for(i=0 ; i<8 ; ++i)
    {
        for(j=0 ; j<8 ; ++j)
        {
            in>>num;
            m_board[i][j] = (type)num;
        }
    }
    //
    in>>m_b;
    file.close();
    return true;
}

void mytest::slot_retract()
{
    retract();
    update();
}

