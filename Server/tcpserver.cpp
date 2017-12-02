#include"tcpserver.h"
//#include<QString>
TcpServer::TcpServer(QWidget *parent)
{
    //创建服务器
    server = new QTcpServer(this);
    //监听本地端口9988,如果出错就输出出错信息
    if(!server->listen(QHostAddress("127.0.0.1"),9988))
    {
        qDebug()<<server->errorString();
        close();
    }
    //连接信号和槽
    //当有新连接时调用newConnection处理
    connect(server,SIGNAL(newConnection()),
            this,SLOT(slotNewConnection()));
}

void TcpServer::slotNewConnection()
{
    if(socketTwo != NULL)
        return;
    if(socketOne == NULL)
    {
        socketOne = new QTcpSocket;
        socketOne = server->nextPendingConnection();
        connect(socketOne,SIGNAL(readyRead()),
                this,SLOT(slotReadyRead()));
        QByteArray buf;
        buf.append(3);
        socketOne->write(buf);		//通知客户端服务器已连接
    }
    else
    {
        socketTwo = new QTcpSocket;
        socketTwo = server->nextPendingConnection();
        connect(socketTwo,SIGNAL(readyRead()),
                this,SLOT(slotReadyRead()));
        //通知双方游戏开始
        //通知第一个连接,游戏开始
        QByteArray buf;
        buf.append(5);
        socketOne->write(buf);
        buf.clear();
        //通知第二个连接,游戏开始,但设置失效
        // buf.append(6);
        //socketTwo->write(buf);
        //通知第二个连接,游戏开始,但设置失效
        //同时把设置信息发送给另一方,
        socketTwo->flush();
        socketTwo->write(bufSets);
    }
#if 0
    while(server->hasPendingConnections())
    {
        //调用nextPendingConnection去获得连接
        tmpSocket = server->nextPendingConnection();
        //为新的socket连接槽函数,处理数据
        connect(tmpSocket,SIGNAL(readyRead()),
                this,SLOT(slotReadyRead()));
        QByteArray buf;
        buf.append(3);
        tmpSocket->write(buf);		//通知客户端服务器已连接
    }
    if(socketTwo != NULL)
    {
        socketTwo->write(bufSets);	//把设置结果通知另一方
        QByteArray buf;
        //通知双方游戏开始
        //通知第一个连接,游戏开始
        buf.append(5);
        socketOne->write(buf);
        buf.clear();
        //通知第二个连接,游戏开始,但设置失效
        buf.append(6);
        socketTwo->write(buf);
    }

#endif
}

void TcpServer::slotReadyRead()
{
    //还有数据时继续读取
    while(socketOne->bytesAvailable()>0)
    {
        QByteArray buf = socketOne->readAll();
        qDebug()<<"Server"<<buf;
        //处理数据,socketOne有设置权
        if(buf.at(0) == 0)
        {
            bufSets.clear();
            bufSets = buf;		//保存设置
        }
        else
            socketTwo->write(buf);
    }
    if(socketTwo != NULL)
    {
        while(socketTwo->bytesAvailable()>0)
        {
            QByteArray buf = socketTwo->readAll();
            qDebug()<<"two"<<buf;
            if(buf.at(0) != 0 )			//不允许第二个连接设置
                socketOne->write(buf);
        }
    }
}



