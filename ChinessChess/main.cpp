
#include<QApplication>
#include"board.h"
#include"mainwindow.h"
#include"selectdialog.h"
#include"mylog.h"
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
    initGLOG(argv[0]);
    LOG(INFO) << "main";
    TRACER;
    //    QApplication::setStyle("motif");
   // Board b;
   // b.setWindowTitle("board");
    MainWindow m;
    //b.show();
    m.show();

  //  selectDialog s;
  //  s.exec();


    return app.exec();
}
