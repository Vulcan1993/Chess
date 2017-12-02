#include<QApplication>
#include"tcpserver.h"

#include<QDebug>
#include<QMessageBox>
int main(int argc,char **argv)
{
    QApplication app(argc,argv);

    TcpServer s;
    s.setWindowTitle("Server");
    s.show();


    return app.exec();
}
