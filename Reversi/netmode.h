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
   void slot_netConnection();
   void slot_readyRead();
   void slot_retract();
private :
   bool m_color;
   bool m_isServer;
   void click(QPoint p);
   
};

#endif // NETMODE_H
