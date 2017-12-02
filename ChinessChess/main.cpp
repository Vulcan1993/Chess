
#include<QApplication>
#include"board.h"
#include"mainwindow.h"
#include"selectdialog.h"
int main(int argc,char **argv)
{
    QApplication app(argc,argv);
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
