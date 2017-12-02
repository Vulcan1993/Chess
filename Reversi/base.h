#ifndef MYTEST_H
#define MYTEST_H

#include"compconfig.h"
#include<QWidget>
#include<QSpacerItem>
#include<QPushButton>
#include<QLayout>
#include<QPainter>
#include<QMouseEvent>
#include<QTimer>
#include<Qpixmap>
#include<QLabel>
#include<QMessageBox>
#include<QFile>
#include<QDataStream>
#define SKYNUM 2000
#include<vector>
#include"selectdialog.h"
using std::vector;
class mytest:public QWidget
{
    Q_OBJECT
public:
    mytest(QWidget *parent=0);
    void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);
    void clearEarning();		//清除提示
    int judge();

    bool winner=false;

    int earning();
    void earn(vector<QPoint> &vp, vector<vector<QPoint> > &vv, bool side, bool state=true);
    virtual int  result();

    //悔棋用的数据结构
    vector<bool> retVB;         //保存每步是谁走
    vector<QPoint> retVP;       //记录每步的点
    vector<vector<QPoint>> retVV;    //记录每步翻过来的点
    virtual void retract();             //悔棋

    void record(vector<QPoint> &v,QPoint p,bool side); //记录每步的状况
    void onepointexec(vector<QPoint> &v, QPoint p, bool side);
    bool write();               //保存游戏状态
    bool read();                //读取游戏状态
protected:
    //保存棋盘状态
    QString fileName = "one";

    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn8;
    QPushButton *btn7;
    QPushButton *btn6;
    QPushButton *btn5;
    QPushButton *btn9;
    QSpacerItem *spacer;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QLabel *label;
    QLabel *label2;
    int d=80;
    bool b=false;
    enum type{kong,black,white,blackcircle,whitecircle};
    type board[8][8];
    QPoint sky[SKYNUM];
    QTimer *timer;
    QPoint fangxiang[8];
signals:
    void signal1();
protected slots:

    void slotEarning();
    virtual void slotRetract();
    void slotrestart();
    void slotWrite();
    virtual void slotRead();
    void slotGoBack();
};


#endif // MYTEST_H
