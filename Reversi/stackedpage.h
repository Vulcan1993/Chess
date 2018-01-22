#ifndef STACKEDPAGE_H
#define STACKEDPAGE_H
#include"compconfig.h"
#include <QWidget>
#include<QLabel>
#include<QComboBox>
#include<QLayout>
//人人对战
class MultiPage : public QWidget
{
    Q_OBJECT
public:
    MultiPage(QWidget *parent=0);

    QLabel *m_lAhead;				//提示,先行

};


//人机对战
class AiPage : public QWidget
{
    Q_OBJECT
public:
    AiPage(QWidget *parent=0);

    void setting(bool & ahead, int & difficulty);

private:
    QLabel *m_lDifficulty;        //提示 ,难度
    QLabel *m_lAhead;             //提示,先行
    QComboBox *m_comDifficulty;
    QComboBox *m_comAhead;
};


//网络模式
class NetPage : public QWidget
{
    Q_OBJECT
public:
    NetPage(QWidget *parent=0);
    //如果是服务器端,返回true,否则false
    void setting(bool &isServer, bool &color);
private:
    QLabel *m_lMode;
    QComboBox *m_comMode;
};

#endif // STACKEDPAGE_H
