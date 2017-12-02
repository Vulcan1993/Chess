#ifndef NETMODE_H
#define NETMODE_H
#include<QTcpServer>
#include<QTcpSocket>
#include<QByteArray>
#include"base.h"
class NetMode : public mytest
{
    Q_OBJECT
public:
   NetMode(bool isServer,bool color);
   QTcpServer *server = NULL;
   QTcpSocket *socket = NULL;
   void mousePressEvent(QMouseEvent *ev);

public slots:
   void slotNetConnection();
   void slotReadyRead();
   void slotRetract();
private :
   bool color;
   bool isServer;
   void click(QPoint p);
   
};

#endif // NETMODE_H
