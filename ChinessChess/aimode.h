#ifndef AIMODE_H
#define AIMODE_H
#include"board.h"

class AiMode : public Board
{
    Q_OBJECT
public:
    AiMode(Camp xian,Camp playerCamp,QWidget *parent=0,int level = 5);
    void mousePressEvent(QMouseEvent *ev);

protected:
    //估值函数指针
    int(AiMode:: *assessTheSituation)(Camp camp);
    //搜索算法指针
    Step *(AiMode:: *searchTheBestStep)();
    //棋子价值数组
    int m_value[32];// = { 500, 300, 150, 100, 1000000, 100, 150, 300, 500, 300, 300, 100, 100, 100, 100, 100,
                  //500,300,150,100,1000000,100,150,300,500,300,300,100,100,100,100,100};

    //方向数组,记录上下左右四个方向的row,col偏移量
    int m_direction[4][2];// = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
    //Camp playerCamp = red;          //记录玩家的阵营
    Camp m_aiCamp ;			//记录电脑阵营


    int m_searchLevel = 7;


protected:
    //静态估值,返回当前棋局的估值,当isRed为true时,返回红方的
    int staticAssess(Camp camp);
    //获得某一阵营的所有合法走步并返回,isRed为true时返回红方的所有走步
    vector<Step *> *getAllStep(Camp camp);
    //生成id棋子的所有合法走步
    vector<Step *> *generateAllSteps(int id);
    //(辅助函数)判断是否越界和自相残杀,返回true表示没有,
    bool checkTwoCamp(const Step *step);

    //生成各种棋子的所有合法走步
    vector<Step *> *generateStepJu(int id);
    vector<Step *> *generateStepMa(int id);
    vector<Step *> *generateStepPao(int id);
    vector<Step *> *generateRedXiang(int id);
    vector<Step *> *generateRedShi(int id);
    vector<Step *> *generateRedJiang(int id);
    vector<Step *> *generateRedZu(int id);
    vector<Step *> *generateBlackXiang(int id);
    vector<Step *> *generateBlackShi(int id);
    vector<Step *> *generateBlackJiang(int id);
    vector<Step *> *generateBlackZu(int id);

    //搜索算法
    //极大极小分析法
    int getMaxScore(int level);
    int getMinScore(int level);
    Step *MinimaxMethod();
    //α-β剪枝
    //β剪枝(电脑走时候剪,根据目前人目前找到的最小值),max为目前人走的,给电脑的最小的值(在一堆最大值里找的最小值)
    //max为上确界,大于max时剪
    int pruningMaxScore(int level, int max);
    //α剪枝(人走时候剪,根据电脑目前找到的最大值剪),min为目前电脑走的,给电脑的最大值(在一堆最小值里找最大值)
    //min为下确界,小于min时剪
    int pruningMinScore(int level,int min);
    Step *pruning();
private slots:
    void slotDebug();
    void slotComputerClick();
    void restart();
    void undoStep();
    void slotStart();

};

#endif // AIMODE_H
