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

    bool m_winner=false;

    int earning();
    void earn(vector<QPoint> &vp, vector<vector<QPoint> > &vv, bool side, bool state=true);
    virtual int  result();

    //悔棋用的数据结构
    vector<bool> m_retVB;         //保存每步是谁走
    vector<QPoint> m_retVP;       //记录每步的点
    vector<vector<QPoint>> m_retVV;    //记录每步翻过来的点
    virtual void retract();             //悔棋

    void record(vector<QPoint> &v,QPoint p,bool side); //记录每步的状况
    void onepointexec(vector<QPoint> &v, QPoint p, bool side);
    bool write();               //保存游戏状态
    bool read();                //读取游戏状态
protected:
    //保存棋盘状态
    QString m_fileName = "one";

    QPushButton *m_btn1;
    QPushButton *m_btn2;
    QPushButton *m_btn3;
    QPushButton *m_btn4;
    QPushButton *m_btn8;
    QPushButton *m_btn7;
    QPushButton *m_btn6;
    QPushButton *m_btn5;
    QPushButton *m_btn9;
    QSpacerItem *m_spacer;
    QVBoxLayout *m_vLayout;
    QHBoxLayout *m_hLayout;
    QLabel *m_label;
    QLabel *m_label2;
    int m_step=80;
    bool m_b=false;
    enum type{kong,black,white,blackcircle,whitecircle};
    type m_board[8][8];
    QPoint m_sky[SKYNUM];
    QTimer *m_timer;
    QPoint m_direction[8];
signals:
    void signal_1();
protected slots:

    void slot_earning();
    virtual void slot_retract();
    void slot_restart();
    void slot_write();
    virtual void slot_read();
    void slot_goBack();
};


#endif // MYTEST_H
