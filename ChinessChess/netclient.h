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
    void slot_tryConnect( Camp player,Camp xian);
    void slot_readyRead();
    void slot_deconnect();
    void slot_writeConnect();
    //悔棋
    void slot_undoStep();
    //重新开始
    void slot_restart();

    void slot_setMode();
//    void slotInitialize();
};


#endif // NETCLIENT_H
