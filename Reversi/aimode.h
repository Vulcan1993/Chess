#ifndef AIMODE_H
#define AIMODE_H

#include"base.h"


class AiMode:public mytest
{
    Q_OBJECT
public:
    AiMode(bool ren,int level);

    int  result();
    //决定人执红还是执黑
    bool ren;
    void mousePressEvent(QMouseEvent *ev);
    //电脑走回去
    void computerMove(vector<QPoint> &vp, vector<vector<QPoint> > &vv,bool state, int i,int);
    //电脑走一步
    void computerMove(vector<QPoint> &vp, vector<vector<QPoint> > &vv, bool state, int i);

    //随机
    void computerClick();
    //一步人工智能
    void computerClick(bool);
    //二步人工智能
    void computerClick(double);

    //n步人工智能
    int level;               //搜索深度
    //极大极小分析法
    int getMaxScore(int level);
    int getMinScore(int level);
    void computerClick(int level);

    //剪枝
    //返回最大估值的一步的估值
    int getMaxScore(int level, int max);
    //返回最小估值
    int getMinScore(int level, int min);
    void computerClick(char level);
    //估值函数,返回黑方棋子数与白方棋子数之差
    int score(bool side);

private slots:
    void slotComputerClick();
    void slotDebug();
    void slotYi();
    void slotRead();
    void slotRetract();

};


#endif // AIMODE_H
