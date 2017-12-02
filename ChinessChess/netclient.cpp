#include"netclient.h"

NetClient::NetClient(Camp xian,Camp playerCamp, QWidget *parent)
    :Board(xian,playerCamp,parent)
{

    QTimer::singleShot(100,this,SLOT(slotInitialize()));
    //创建套接字
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),
            this,SLOT(slotReadyRead()));

}

void NetClient::mousePressEvent(QMouseEvent *ev)
{

    //游戏结束后不允许再走
    if(gameOver)
        return;
    //不该本方走的时候不能随便点击
    if(alternate != playerCamp)
        return;
    int row,col;
    //把点击的点转换为数组对应的行列值
    coordinateToRow(ev->pos(),row,col);
    //把点击的点转换成棋盘数组对应的点
    if(playerCamp == black)
        toClientRow(row,col);

    //把点击的点传给对方,
    QByteArray buf;
    buf.append(1);
    buf.append(row);
    buf.append(col);
    socket->write(buf);

    if(ev->button() == Qt::LeftButton)
    {
        //处理点击事件
        mouseClick(row,col);
    }
}

void NetClient::mouseClick(int row, int col)
{
    if(playerClick(row,col))
    {
        //当确实走棋后判断是否分出胜负
        Camp winner;
        if(whetherTheGameIsOver(winner))
        {
            //已分出胜负
            gameOver = true;
            if(winner == red)
            {
                QTimer::singleShot(300,this,SLOT(slotShowRedVictory()));
            }
            else
            {
                QTimer::singleShot(300,this,SLOT(slotShowBlackVictory()));
            }
        }
        update();
    }
}



void NetClient::slotTryConnect( Camp player,Camp xian)
{  
    //连接服务器,
    socket->connectToHost("127.0.0.1",9988);

#if 1
    //发送设置信息
    QByteArray buf;
    buf.append((char)0);

    if(player == red)       //自己用红子
        buf.append(1);      //对方用黑子
    else
        buf.append((char)0);
    if(xian == red)
        buf.append((char)0);
    else
        buf.append(1);

    socket->write(buf);
#endif
    //socket->flush();
#if 0
        qDebug()<<"connect"<<socket->errorString();
    else
        qDebug()<<"connect true"<<socket->errorString();
#endif

}
//数据到达时接收数据,
//如果第一字节是0,表示设置执红执黑先行等问题
//如果第一个字节是1,表示下子,(三个字节依次为,1,row,col)
//是2表示悔棋申请,(只有一个字节)
//是3时,是通知客户端已连接
//是4时,协调悔棋信息,第二字节是1时同意,2时不同意
//  5 , 游戏开始
//  6 , 重新开始申请
//  7 , 重新开始申请结果 7 1 同意 7 2 不同意


void NetClient::slotReadyRead()
{
    //还有数据时继续读取
    while(socket->bytesAvailable()>0)
    {
        QByteArray buf = socket->readAll();
        qDebug()<<"client"<<buf;
        switch(buf.at(0))
        {
        case 0:
            QMessageBox::information(this,"游戏开始","设置失效(已有设置)\n已加入棋局,游戏开始",
                                     QMessageBox::Ok);


            switch(buf.at(1))
            {
            case 0:
                if(buf.at(2) == 0)
                {
                    //0 0 0 表示执红先行
                    xianCamp = red;
                    playerCamp = red;
                    initImage(playerCamp,2);
                    Board::restart();


                    //"默认模式 : 人人对战 , 执红 , 红方先行  "
                    QString str = "当前模式 : 网络对战 , ";
                    if(playerCamp == red)
                        str += "执红 , ";
                    else
                        str += "执黑 , ";
                    if(alternate == red)
                        str += "红方先行  ";
                    else
                        str +="黑方先行";
                    emit changeMode(str);

                    update();
                }
                else
                {
                    //0 0 1 表示执红后行
                    xianCamp = black;
                    playerCamp = red;
                    initImage(playerCamp,2);
                    Board::restart();


                    //"默认模式 : 人人对战 , 执红 , 红方先行  "
                    QString str = "当前模式 : 网络对战 , ";
                    if(playerCamp == red)
                        str += "执红 , ";
                    else
                        str += "执黑 , ";
                    if(alternate == red)
                        str += "红方先行  ";
                    else
                        str +="黑方先行";
                    emit changeMode(str);
                    update();
                }
                break;
            case 1:
                if(buf.at(2) == 0)
                {
                    //0 1 0 表示执黑先行
                    xianCamp = red;
                    playerCamp = black;
                    initImage(playerCamp,2);
                    Board::restart();

                    //"默认模式 : 人人对战 , 执红 , 红方先行  "
                    QString str = "当前模式 : 网络对战 , ";
                    if(playerCamp == red)
                        str += "执红 , ";
                    else
                        str += "执黑 , ";
                    if(alternate == red)
                        str += "红方先行  ";
                    else
                        str +="黑方先行";
                    emit changeMode(str);

                    update();
                }
                else
                {
                    //0 1 1 表示执黑后行
                    xianCamp = red;
                    playerCamp = black;
                    initImage(playerCamp,2);
                    Board::restart();
                    //"默认模式 : 人人对战 , 执红 , 红方先行  "
                    QString str = "当前模式 : 网络对战 , ";
                    if(playerCamp == red)
                        str += "执红 , ";
                    else
                        str += "执黑 , ";
                    if(alternate == red)
                        str += "红方先行  ";
                    else
                        str +="黑方先行";
                    emit changeMode(str);
                    update();
                }
                break;
            }
            gameOver = false;
            if(alternate == red)
                emit changeState("当前状态 : 红方下");
            else
                emit changeState("当前状态 : 黑方下");

    //        QTimer::singleShot(100,this,SLOT(slotSetMode()));
            break;
        case 1:
            int row,col;
            //发送过来的row,col都是处理过的
            row = buf.at(1);
            col = buf.at(2);
            mouseClick(row,col);
            update();
            break;
        case 2:
            if(QMessageBox::Yes ==  QMessageBox::question(this,"悔棋","同意对方悔棋?",
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::Yes))
            {
                Board::undoStep();
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
                QMessageBox::information(this,"连接结果","服务器已连接",
                                          QMessageBox::Ok);
            break;
        case 4:
            if(buf.at(1) == 2)
            //if(buf[1] == 2)
                {
                    QMessageBox::information(this,"悔棋结果","对方不同意悔棋,不能悔棋",
                                              QMessageBox::Ok);
                }
                else
                {
                    Board::undoStep();
                }
            break;
        case 5:
            QMessageBox::information(this,"游戏开始","已加入棋局,游戏开始",
                                     QMessageBox::Ok);
            gameOver = false;
            if(alternate == red)
                emit changeState("当前状态 : 红方下");
            else
                emit changeState("当前状态 : 黑方下");


            break;
#if 1
        case 6:			//重新开始申请
            if(QMessageBox::Yes ==  QMessageBox::question(this,"Message","是否同意对方重新开始?",
                                  QMessageBox::Yes|QMessageBox::No,
                                  QMessageBox::Yes))
            {           //通知对方
                Board::restart();       //调用父类函数重新开始
                QByteArray buf;
                buf.append(7);
                buf.append(1);
                socket->write(buf);
            }
            else
            {
                QByteArray buf;
                buf.append(7);
                buf.append(2);
                socket->write(buf);
            }
#endif
            break;

        case 7:
            if(buf.at(1) == 1)      //同意重新开始
            {
                Board::restart();
            }
            else                    //不同意重新开始
            {
                QMessageBox::information(this,"Message","对方不允许重新开始,不能重新开始",
                                         QMessageBox::Ok);
            }

        }
    }
}
void NetClient::slotDeconnect()
{
    socket->close();
}

void NetClient::slotWriteConnect()
{
#if 0
  //发送设置信息
    QByteArray buf;
    buf.append((char)7);
    if(xian == red)
        buf.append((char)0);
    else
        buf.append(1);
    if(player == red)       //自己用红子
        buf.append(1);      //对方用黑子
    else
        buf.append((char)0);

    socket->write(buf);
#endif
    QByteArray buf;
    static int i=0;
    buf.append(i++);
    socket->write(buf);
}

void NetClient::undoStep()
{
    //申请悔棋
    QByteArray buf;
    buf.append(2);
    socket->write(buf);
}
//重新开始申请
void NetClient::restart()
{
    QByteArray buf;
    buf.append(6);
    socket->write(buf);
}

void NetClient::slotSetMode()
{
    //"默认模式 : 人人对战 , 执红 , 红方先行  "
    QString str = "当前模式 : 网络对战 , ";
    if(playerCamp == red)
        str += "执红 , ";
    else
        str += "执黑 , ";
    if(alternate == red)
        str += "红方先行  ";
    else
        str +="黑方先行";
    emit changeMode(str);
}

//void NetClient::slotInitialize()
//{
//    if(alternate == red)
//        emit changeState("当前状态 : 红方下");
//    else
//        emit changeState("当前状态 : 黑方下");
//}

