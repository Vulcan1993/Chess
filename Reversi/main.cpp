
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
