#ifndef STACKEDPAGE_H
#define STACKEDPAGE_H

#include <QWidget>
#include<QLabel>
#include<QComboBox>
#include<QLayout>
#include"chessman.h"
//人人对战
//enum Camp {red=0,black};
class MultiPage : public QWidget
{
    Q_OBJECT
public:
    MultiPage(QWidget *parent=0);

    QLabel *lAhead;
    QLabel *lCamp;				//提示,阵营
    QComboBox *cAhead;
    QComboBox *comCamp;
    void setting(Camp &xian,Camp &player);

};


//人机对战
class AiPage : public QWidget
{
    Q_OBJECT
public:
    AiPage(QWidget *parent=0);

    void setting(Camp &xian,Camp &player,int & difficulty);

private:
    QLabel *lDifficulty;        //提示 ,难度
    QLabel *lCamp;				//提示,阵营
    QLabel *lAhead;             //提示,先行
    QComboBox *comDifficulty;
    QComboBox *comCamp;
    QComboBox *comAhead;
};


//网络模式
class NetPage : public QWidget
{
    Q_OBJECT
public:
    NetPage(QWidget *parent=0);
    //如果是服务器端,返回true,否则false
    void setting(Camp &player,Camp &xian);
private:
    QLabel *lCamp;				//提示,阵营
    QLabel *lAhead;             //提示,先行
    QComboBox *comCamp;
    QComboBox *comAhead;
};

#endif // STACKEDPAGE_H

