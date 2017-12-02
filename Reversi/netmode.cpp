#include"netmode.h"
//isServer是true时是服务器,color为false时是执黑(先行)
NetMode::NetMode(bool isServer, bool color)
{
    //btn3->setEnabled(false);
    btn3->setVisible(false);
    //btn4->setEnabled(false);
    btn4->setVisible(false);
    this->color = color;
    if(color == true)
        label2->setText("执白");
    else
        label2->setText("执黑");
    connect(btn2,SIGNAL(clicked()),this,SLOT(slotRetract()));

    this->isServer = isServer;
    if(isServer)
    {
        //创建服务器
        server = new QTcpServer;
        //监听本地端口9988,如果出错就输出错误信息并关闭
        if(!server->listen(QHostAddress::Any,9988))
        {
            qDebug()<<server->errorString();
            close();
        }
        //当客户端连接时,调用slotNetConnection槽
        connect(server,SIGNAL(newConnection()),
                this,SLOT(slotNetConnection()));
        label->setText("当前状态: 等待连接 ....");
    }
    //客户端
    else
    {
        socket = new QTcpSocket(this);
        //连接
        socket->connectToHost("127.0.0.1",9988);
        label->setText("当前状态: 连接中 ....");
        connect(socket,SIGNAL(readyRead()),this,SLOT(slotReadyRead()));
    }
}

void NetMode::mousePressEvent(QMouseEvent *ev)
{
    if(winner)
        return;

    if(b != color)
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
        click(p);
        //判断是否结束
        result();
        update();
        //把数据发送给另一方
        QByteArray buf;
        buf.append(1);
        buf.append(i);
        buf.append(j);
        socket->write(buf);

    }//leftbutton

    if(ev->button()==Qt::RightButton)
        board[i][j]=kong;
    update();
}




void NetMode::slotNetConnection()
{
    //判断是否已连接,网络对战是两个人,
    if(socket == NULL)
    {
        //调用nextPendingConnection去获得连接
        if(!(socket = server->nextPendingConnection()))
            qDebug()<<socket->errorString();

        //为新的socket提供槽函数,来接收数据
        connect(socket,SIGNAL(readyRead()),
                this,SLOT(slotReadyRead()));
        if(b == true)
            label->setText("当前状态: 已连接, 白子走");
        else
            label->setText("当前状态: 已连接, 黑子走");
      //  label->setText("当前状态: 已连接");
        //通知对方已连接
        QByteArray buf;
        buf.append(3);
        if(!socket->write(buf))
            qDebug()<<socket->errorString();
        update();
    }
}
//数据到达时接收数据,
//如果第一个字节是1,表示下子,(三个字节依次为,1,row,col)
//是2表示悔棋申请,(只有一个字节)
//是3时,是通知客户端已连接
//是4时,协调悔棋信息,第二字节是1时同意,2时不同意
void NetMode::slotReadyRead()
{
    //服务器用白子,客户端用黑子
   //创建一个字节数组,读取所有数据
    QByteArray buf = socket->readAll();
    int i = buf.size();
#if 0
    if(i==1)
    {
            qDebug()<<isServer<<"    \t"<<buf[0];
    }
    else
    {
        if(i == 2)
        {
            qDebug()<<isServer<<"    \t"<<buf[0]<<"   "<<buf[1];
        }
        else
        {
            qDebug()<<isServer<<"    \t"<<buf[0]<<"   "<<buf[1]<<"   "<<buf[2];

        }
    }
#endif
    switch(buf[0])
    {
    case 1:
        click(QPoint(buf[1],buf[2]));
        result();

        break;
    case 2:
        if(QMessageBox::Yes ==  QMessageBox::question(this,"悔棋","同意对方悔棋?",
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes))
        {
            retract();
            QByteArray buf1;
            buf1.append(4);
            buf1.append(1);         //同意
            socket->write(buf1);
        }
        else
        {
            QByteArray buf1;
            buf1.append(4);
            buf1.append(2);         //不同意
            socket->write(buf1);
        }
        break;
    case 3:
        if(b == true)
            label->setText("当前状态: 已连接, 白子走");
        else
            label->setText("当前状态: 已连接, 黑子走");
        break;
    case 4:
        //if(buf[1] == 2)
        if(buf.at(1) == 2)
        {
            QMessageBox::information(this,"悔棋结果","对方不同意悔棋,不能悔棋",
                                      QMessageBox::Ok);
        }
        else
        {
            retract();
        }
        break;
    }
    update();
}

void NetMode::slotRetract()
{
    QByteArray buf;
    buf.append(2);
    socket->write(buf);
}

void NetMode::click(QPoint p)
{
    int i,j;
    i=p.x();
    j=p.y();
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
    }
}

