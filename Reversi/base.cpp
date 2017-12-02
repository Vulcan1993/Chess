#include"base.h"
#include<QDebug>
mytest::mytest(QWidget *parent)
    :QWidget(parent)
{   //窗口控件布局
    btn1=new QPushButton("开始",this);
    btn2=new QPushButton("悔棋",this);
    btn3=new QPushButton("保存",this);
    btn4=new QPushButton("读取",this);

    btn5=new QPushButton("返回",this);

    btn6=new QPushButton("一步",this);
    btn7=new QPushButton("",this);
    btn8=new QPushButton("提示可下",this);
    btn9=new QPushButton("重新开始",this);

    //用label显示状态
    label = new QLabel("就绪");
    label2 = new QLabel;
    //设置QLabel风格
    label->setStyleSheet("QLabel{font:bold 16px; color:red}");
    label2->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    spacer= new QSpacerItem(640,640);
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addItem(spacer);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(label);
    hlay->addWidget(label2);
    vlay->addLayout(hlay);

    vLayout= new QVBoxLayout;
    vLayout->addWidget(btn1);
    vLayout->addWidget(btn2);
    vLayout->addWidget(btn3);
    vLayout->addWidget(btn4);
    vLayout->addWidget(btn5);
    vLayout->addWidget(btn6);
    vLayout->addWidget(btn7);
    vLayout->addWidget(btn8);
    vLayout->addWidget(btn9);

    vLayout->addStretch(1);

    hLayout =new QHBoxLayout;
    //  hLayout->addItem(spacer);
    hLayout->addLayout(vlay);
    hLayout->addLayout(vLayout);

    setLayout(hLayout);

    timer= new QTimer;
    timer->setInterval(500);
    // connect(timer,SIGNAL(timeout()),this,SLOT(paint()));

    timer->start();

    //初始化棋盘
    int i,j;
    for(i=0 ; i<8 ; ++i)
    {
        for( j=0 ; j<8 ; ++j)
        {
            //强制类型转换，把int转换为mytest::type类型(实际值是相同的)
            board[i][j] =static_cast<mytest::type>(0);
            // board[i][j] =static_cast<mytest::type>(qrand()%3);
        }
    }

    board[3][3]=white;board[4][4]=white;
    board[3][4]=black;board[4][3]=black;




    //connect(btn1,SIGNAL(clicked()),this,SIGNAL(signal1()));
    //connect(this,SIGNAL(signal1()),this,SLOT(one()));

    //connect(btn1,SIGNAL(clicked(bool)),this,SLOT(one()));//与上面的两行功能相同

 //   connect(btn2,SIGNAL(clicked(bool)),btn1,SIGNAL(clicked(bool)));// 让btn2与btn1功能相同
   // connect(btn3,SIGNAL(clicked(bool)),this,SLOT(two()));
    //connect(btn5,SIGNAL(clicked(bool)),this,SLOT(slot3_3()));
    //connect(btn6,SIGNAL(clicked(bool)),this,SLOT(slot1_5()));
    //connect(btn7,SIGNAL(clicked(bool)),this,SLOT(slot5_1()));

    connect(btn3,SIGNAL(clicked(bool)),this,SLOT(slotWrite()));
    connect(btn4,SIGNAL(clicked(bool)),this,SLOT(slotRead()));
    connect(btn5,SIGNAL(clicked(bool)),this,SLOT(slotGoBack()));
    connect(btn8,SIGNAL(clicked(bool)),this,SLOT(slotEarning()));
    connect(btn9,SIGNAL(clicked(bool)),this,SLOT(slotrestart()));
    connect(btn2,SIGNAL(clicked()),this,SLOT(slotRetract()));
    connect(btn5,SIGNAL(clicked(bool)),this,SLOT(slotDebug()));

    //初始化方向数组
    fangxiang[0]=QPoint(-1,0);		//上
    fangxiang[1]=QPoint(1,0);		//下
    fangxiang[2]=QPoint(0,-1);		//左
    fangxiang[3]=QPoint(0,1);		//右
    fangxiang[4]=QPoint(-1,-1);		//左上
    fangxiang[5]=QPoint(1,-1);		//左下
    fangxiang[6]=QPoint(-1,1);		//右上
    fangxiang[7]=QPoint(1,1);		//右下
 //earning(true);
 label->setText("当前状态:黑子下 ");

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
            switch(board[i][j])
            {
            case black:
                pen.setColor(Qt::black);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.setBrush(QBrush(Qt::black));
                painter.drawEllipse(j*d+5,i*d+5,70,70);
                break;
            case white:
                pen.setColor(Qt::white);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.setBrush(QBrush(Qt::white));
                painter.drawEllipse(j*d+5,i*d+5,70,70);
                break;
            case whitecircle:
                pen.setColor(Qt::white);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.setBrush(QBrush(QColor(0,0,0,0)));
                painter.drawEllipse(j*d+5,i*d+5,70,70);
                break;
            case blackcircle:
                pen.setColor(Qt::black);
                pen.setWidth(2);
                painter.setPen(pen);
                painter.setBrush(QBrush(QColor(0,0,0,0)));
                painter.drawEllipse(j*d+5,i*d+5,70,70);
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
            if(board[i][j] == whitecircle || board[i][j] == blackcircle)
                board[i][j] = kong;
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
            if(board[i][j]==white)
                sum1++;
            if(board[i][j]==black)
                sum2++;

        }
    }
    label->setText(QString("游戏结束：黑方%1个子，白子%2个子").arg(sum2).arg(sum1));
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
    if(winner)
        return;
    int i,j;
    j=ev->pos().x()/d;
    i=ev->pos().y()/d;
    QPoint p(i,j);
    //不在棋盘内
    if(i<0||i>8||j<0||j>8)
        return;


    if(ev->button()==Qt::LeftButton)
    {
        vector<QPoint> v;
        onepointexec(v,p,b);
        if(0 != v.size())
        {
            //存储这一步
            record(v,p,b);
            if(b==true)
                board[i][j]=white;
            else
                board[i][j]=black;
            for(QPoint a:v)
            {
                if(b==true)
                    board[a.x()][a.y()]=white;
                else
                    board[a.x()][a.y()]=black;
            }
            //该另一方走了
            //清除之前的提示
            clearEarning();
            result();
        }
    }//leftbutton

    if(ev->button()==Qt::RightButton)
        board[i][j]=kong;
    update();
}









void mytest::slotEarning()
{
    earning();
    update();
}

void mytest::slotrestart()
{
    winner=false;
    retVB.clear();
    retVP.clear();
    retVV.clear();
    //初始化棋盘
    int i,j;
    for(i=0 ; i<8 ; ++i)
    {
        for( j=0 ; j<8 ; ++j)
        {
            //强制类型转换，把int转换为mytest::type类型(实际值是相同的)
            board[i][j] =static_cast<mytest::type>(0);
            // board[i][j] =static_cast<mytest::type>(qrand()%3);
        }
    }
    b=false;
    label->setText("当前状态: 黑子下");
    board[3][3]=white;board[4][4]=white;
    board[3][4]=black;board[4][3]=black;
    update();
}

void mytest::slotWrite()
{
    if(write() == true)
        label->setText("保存成功");
    else
        label->setText("保存失败");
}

void mytest::slotRead()
{
    QString str;
    if(read() == true)
        str="读取成功";
    else
    {
        str ="读取失败";
        return;
    }
    if(b == true)
        str += " : 白方下";
    else
        str += " : 黑方下";
    label->setText(str);
    update();
}

void mytest::slotGoBack()
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

    earn(vp,vv,b);
    for(QPoint a:vp)
    {
        if(b==true)
            board[a.x()][a.y()]=whitecircle;
        else
            board[a.x()][a.y()]=blackcircle;
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
    b = !b;
    if(b == true)
        label->setText("当前状态: 白子下");
    else
        label->setText("当前状态: 黑子下");

    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,b);
    if(vp.empty())
    {
        if(b == true)
            {
                label->setText("当前状态:白子轮空,黑子下");
                i = 1;
            }
        else
            {
                label->setText("当前状态: 黑子轮空,白子下");
                i = 1;
            }
        vp.clear();
        //b取反，判断另一方是否能走
        b = !b;
        earn(vp,vv,b);
        if(vp.empty())
        {
            ////两者皆空,结束,则判断
            int r=judge();
            winner=true;
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
    if(retVP.empty())
        return;
    int i=retVP.size()-1;
    b = retVB[i];
    board[retVP[i].x()][retVP[i].y()] = kong;
    for(QPoint p : retVV[i])
    {
        if(retVB[i] == true)
            board[p.x()][p.y()] = black;
        else
            board[p.x()][p.y()] = white;
    }
    retVB.pop_back();
    retVV.pop_back();
    retVP.pop_back();
    if(b == true)
        label->setText("当前状态: 白子下");
    else
        label->setText(("当前状态: 黑子下"));

}

void mytest::record(vector<QPoint> &v, QPoint p, bool side)
{
    retVV.push_back(v);
    retVP.push_back(p);
    retVB.push_back(side);
}

//如果该点能下,则v里边保存所有能翻的点
void mytest::onepointexec(vector<QPoint> &v,QPoint p,bool side)
{
    int i = p.x();
    int j = p.y();
    type color;
    type colorDuiFang;
    //不在棋盘内
    if(board[i][j] != white && board[i][j] != black)
    {
        if(side==true)
        {
            board[i][j]=white;
            color = white;
            colorDuiFang = black;
        }
        if(side!=true)
        {
            board[i][j]=black;
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
        x = i + fangxiang[fx].x();
        y = j + fangxiang[fx].y();
        while(true)
        {
            //如果越界,则退出
            if(x <0 || x>7 || y<0 || y>7)
                break;
            //找到本方向同色的棋子即退出
            if(board[x][y] == color)
                break;
            if(board[x][y] == kong || board[x][y] == whitecircle || board[x][y] == blackcircle)
                break;
            //检测下一个点
            x = x + fangxiang[fx].x();
            y = y + fangxiang[fx].y();
        }
        //如果没越界则判断
        if(!(x <0 || x>7 || y<0 || y>7))
        {
            //如果是因为找到同色的棋子而推出的
            if(board[x][y] == color)
            {
                //检测是否相邻,如果不相邻,则(x,y)和(i,j)即为边界点
                if(x != i+fangxiang[fx].x() || y != j+fangxiang[fx].y())
                {
                    int t1,t2;
                    t1 = i+fangxiang[fx].x();
                    t2 = j+fangxiang[fx].y();
                    while(t1 != x || t2 != y)
                    {
                        v.push_back(QPoint(t1,t2));
                        t1 = t1 + fangxiang[fx].x();
                        t2 = t2 + fangxiang[fx].y();
                    }
                }
            }
        }
    }
    board[i][j]=kong;
}

bool mytest::write()
{
    QFile file(fileName);
    file.open(QIODevice::ReadWrite);
    QDataStream out(&file);
    //保存棋盘
    int i,j;
    for(i=0 ; i<8 ; ++i)
    {
        for(j=0 ; j<8 ; ++j)
        {
            out<<board[i][j];
        }
    }
    //保存该谁走了
    out<<b;
    file.close();
    return true;
}

bool mytest::read()
{
    QFile file(fileName);
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
            board[i][j] = (type)num;
        }
    }
    //
    in>>b;
    file.close();
    return true;
}

void mytest::slotRetract()
{
    retract();
    update();
}

