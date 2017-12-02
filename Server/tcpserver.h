#ifndef TCPSERVER_H
#define TCPSERVER_H
#include<QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QTimer>
#include<vector>
#define MAXBOARDNUM 1
using std::vector;
#include<array>
using std::array;



class TcpServer : public QWidget
{
    Q_OBJECT
public :
    TcpServer(QWidget *parent = 0);
    QTcpServer *server;
    //最多创建的棋局数
    QTcpSocket * socketOne = NULL;
    QTcpSocket * socketTwo = NULL;
    QByteArray bufSets;
public slots:
    void slotNewConnection();
    void slotReadyRead();

};

#endif // TCPSERVER_H


