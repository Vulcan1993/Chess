
#include<QApplication>
#include<QTime>
#include"base.h"
#include"aimode.h"
#include"netmode.h"
#include"selectdialog.h"


int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    selectDialog selDlg;
    selDlg.resize(400,300);
    //模态显示
    selDlg.show();
    return app.exec();
}



#if 0
int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
 #if 1

    if(QMessageBox::Yes == QMessageBox::question(0,
                         ("Question"),
                         ("作为服务器启动?"),
                         QMessageBox::Yes | QMessageBox::No,
                         QMessageBox::Yes))
    {
        NetMode *s = new NetMode(true,false);
        s->setWindowTitle("Server");
        s->show();
    }
    else
    {
        NetMode *c = new NetMode(false,true);
        c->setWindowTitle("Client");
        c->show();
    }
#endif
#if 0

 qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //mytest f;
 //false是人走黑子
    AiMode f(false,6);
    f.setMaximumSize(800,600);
    f.setWindowTitle("黑白棋");
//   f.resize(600,600);
    f.show();
#endif
    return app.exec();
}


#endif
