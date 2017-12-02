#ifndef NETCLIENT_H
#define NETCLIENT_H
#include"compconfig.h"

#include<QWidget>
#include<QTcpSocket>
#include<QByteArray>
#include"board.h"
#include"chessman.h"

class NetClient : public Board
{
    Q_OBJECT
public:
    NetClient(Camp xian,Camp playerCamp,QWidget *parent=0);

    QTcpSocket *socket;
    void mousePressEvent(QMouseEvent *ev);
    //处理鼠标点击
    void mouseClick(int row, int col);

public slots:
    void slotTryConnect( Camp player,Camp xian);
    void slotReadyRead();
    void slotDeconnect();
    void slotWriteConnect();
    //悔棋
    void undoStep();
    //重新开始
    void restart();

    void slotSetMode();
//    void slotInitialize();
};


#endif // NETCLIENT_H
