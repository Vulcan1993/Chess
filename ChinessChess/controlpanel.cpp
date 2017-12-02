#include "controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent)
{

    btn1 = new QPushButton("选择模式");
    btn2 = new QPushButton("开始");
    btn3 = new QPushButton("重新开始");
    btn4 = new QPushButton("悔棋");
    btn5 = new QPushButton("保存");
    btn6 = new QPushButton("载入");
    btn7 = new QPushButton("退出");
  //  btn8 = new QPushButton("AiMode");
    QVBoxLayout *vLay = new QVBoxLayout(this);
    vLay->addWidget(btn1);
    vLay->addWidget(btn2);
    vLay->addWidget(btn3);
    vLay->addWidget(btn4);
    vLay->addWidget(btn5);
    vLay->addWidget(btn6);
    vLay->addWidget(btn7);
//    vLay->addWidget(btn8);
}
