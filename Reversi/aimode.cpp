#include"aimode.h"
#include<QDebug>
#include<windows.h>
#define debug_one 0
#define debug_two 0


AiMode::AiMode(bool ren, int level)
{
    //确定人走哪方的子
    this->ren = ren;
    if(ren == true)
        label2->setText("电脑执黑");
    else
        label2->setText("电脑执白");
    //确搜索深度
    this->level = level;
    //true表示白子
    if(ren == true)
        slotComputerClick();
    //连接,实现一步对战两步
    connect(btn6,SIGNAL(clicked(bool)),this,SLOT(slotYi()));
}

#if 1
int AiMode::result()
{
    if(mytest::result() == 1 && winner == false)
    {
        //如果不该人走了,则让电脑走
        if(b != ren)
            QTimer::singleShot(1000,this,SLOT( slotComputerClick()));
        update();
    }
    return 0;
}
#endif

//vp里放所有可走的点,vv里放所有应翻的点
//state==true时,是提示,state == false 时,是电脑分析时用的
//side 为true时，找white的，


void AiMode::mousePressEvent(QMouseEvent *ev)
{
    if(winner)
        return;
    if(b == !ren)
        return;
    int i,j;
    j=ev->pos().x()/d;
    i=ev->pos().y()/d;
    QPoint p(i,j);
    //不在棋盘内
    if(i<0||i>8||j<0||j>8)
        return;


    if(ev->button()==Qt::LeftButton)
    {
        vector<QPoint> v;
        onepointexec(v,p,b);
        if(0 != v.size())
        {
            //存储这一步
            record(v,p,b);
            if(b==true)
                board[i][j]=white;
            else
                board[i][j]=black;
            for(QPoint a:v)
            {
                if(b==true)
                    board[a.x()][a.y()]=white;
                else
                    board[a.x()][a.y()]=black;
            }
            update();
        //该另一方走了
            //清除之前的提示
            clearEarning();
            result();
           QTimer::singleShot(1000,this,SLOT( slotComputerClick()));
            //Sleep(100);
          //  computerClick();
         //   b = !b;

        }
    }//leftbutton

    if(ev->button()==Qt::RightButton)
        board[i][j]=kong;
    update();

}




//走回去
void AiMode::computerMove(vector<QPoint> &vp, vector<vector<QPoint> > &vv, bool state,int i, int)
{
    //消除白方的走步
    if(state == true)
    {
        board[vp[i].x()][vp[i].y()] = kong;
        for(QPoint p : vv[i])
        {
            board[p.x()][p.y()] = black;
        }

    }
    //消除黑方的走步
    else
    {
        board[vp[i].x()][vp[i].y()] = kong;
        for(QPoint p : vv[i])
        {
            board[p.x()][p.y()] = white;
        }
    }
}
//走棋
void AiMode::computerMove(vector<QPoint> &vp,vector<vector<QPoint>> &vv,bool state,int i)
{
    if(state == true)
    {
        //下这个点
        board[vp[i].x()][vp[i].y()] = white;
        //翻子
        for(QPoint p : vv[i])
        {
            board[p.x()][p.y()] = white;
        }

    }
    else
    {
        board[vp[i].x()][vp[i].y()] = black;
        for(QPoint p : vv[i])
        {
            board[p.x()][p.y()] = black;
        }
    }
}



//随机
void AiMode::computerClick()
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;

    earn(vp,vv,b,false);
    if(vp.empty())
        return;
    int i = qrand()%vp.size();

    computerMove(vp,vv,b,i);

        record(vv[i],vp[i],b);

}

//一步人工智能
void AiMode::computerClick(bool)
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,b,i);
        //估值
        scoreTmp = score(true);
        //找到最大估值,最好的一步棋
        if(maxScore < scoreTmp)
        {
            maxScore = scoreTmp;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,b,i,0);
    }
    //走最好的一步
    if(maxStep!= -1)
    {
        computerMove(vp,vv,b,maxStep);
        record(vv[maxStep],vp[maxStep],b);
    }
    else
    {
        computerClick();
    }

    update();
}

//两步人工智能
void AiMode::computerClick(double)
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;
    int scoreTmp=-10000;
    int minScore = 10000;

    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设电脑走一步,翻子
        computerMove(vp,vv,b,i);

        vector<QPoint> vpr;
        vector<vector<QPoint>> vvr;
        earn(vpr,vvr,!b,false);
        minScore = 10000;
        //int minStep = -1;
        for(int i=0 ; i<vpr.size() ; ++i)
        {
            //假设人走一步,翻子
            computerMove(vpr,vvr,!b,i);
            //估值
            scoreTmp = score(false);
            //找到最小值，对电脑最不利的局面
            if(minScore > scoreTmp)
            {
                minScore = scoreTmp;
            }
            //人再走回去
            computerMove(vpr,vvr,!b,i,0);
        }
        if(maxScore < minScore)
        {
            maxScore = minScore;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,b,i,0);
    }
    //走最好的一步
    if(maxStep != -1)
    {
        computerMove(vp,vv,b,maxStep);
        record(vv[maxStep],vp[maxStep],b);
    }
    else
    {
        computerClick(true);
    }
    update();
}

int AiMode::getMaxScore(int level)
{
    if(level <= 0)
        return score(true);
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMinScore(level - 1);

        //找到最大估值,最好的一步棋
        if(maxScore < scoreTmp)
        {
            maxScore = scoreTmp;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,b,i,0);
    }
    return maxScore;
}

int AiMode::getMinScore(int level)
{
    if(level <= 0)
        return score(true);
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,!b,false);
    //记录最大估值
    int minScore = 10000;
    //记录最好的一步棋的下标

    int scoreTmp=10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,!b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMaxScore(level - 1);

        //找到最大估值,最好的一步棋
        if(minScore > scoreTmp)
        {
            minScore = scoreTmp;
        }
        //走回去
        computerMove(vp,vv,!b,i,0);
    }
    return minScore;
}

void AiMode::computerClick(int level)
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMinScore(level - 1);
        //找到最大估值,最好的一步棋
        if(maxScore < scoreTmp)
        {
            maxScore = scoreTmp;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,b,i,0);
    }
    //走最好的一步
    if(maxStep!= -1)
    {
        computerMove(vp,vv,b,maxStep);
        record(vv[maxStep],vp[maxStep],b);
    }
    else
    {
        computerClick(double(0));
       // computerClick();
    }
    update();
}


//剪枝
//n步人工智能
int AiMode::getMaxScore(int level,int max)
{
    if(level <= 0)
        return score(true);
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMinScore(level - 1,maxScore);
        //β剪枝
        if(scoreTmp > max)
        {
            //走回去
            computerMove(vp,vv,b,i,0);
            return scoreTmp;
        }
        //找到最大估值,最好的一步棋
        if(maxScore < scoreTmp)
        {
            maxScore = scoreTmp;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,b,i,0);
    }
    return maxScore;
}

int AiMode::getMinScore(int level,int min)
{
    if(level <= 0)
        return score(true);
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,!b,false);
    //记录最大估值
    int minScore = 10000;
    //记录最好的一步棋的下标

    int scoreTmp=10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,!b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMaxScore(level - 1,minScore);
        //α剪枝
        if(scoreTmp < min)
        {
            //走回去
            computerMove(vp,vv,!b,i,0);
            return scoreTmp;
        }
        //找到最大估值,最好的一步棋
        if(minScore > scoreTmp)
        {
            minScore = scoreTmp;
        }
        //走回去
        computerMove(vp,vv,!b,i,0);
    }
    return minScore;
}



void AiMode::computerClick(char level)
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMinScore(level - 1,maxScore);
        //找到最大估值,最好的一步棋
        if(maxScore < scoreTmp)
        {
            maxScore = scoreTmp;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,b,i,0);
    }
    //走最好的一步
    if(maxStep!= -1)
    {
        computerMove(vp,vv,b,maxStep);
        record(vv[maxStep],vp[maxStep],b);
    }
    else
    {
        computerClick();
    }
    update();
}
//side 为true时是求白方的估值,false时是求黑方的估值
int AiMode::score(bool side)
{
    int i,j,sum1=0,sum2=0;
    for(i=1;i<7;i++)
    {
        for(j=1;j<7;j++)
        {
            if(board[i][j]==white)
                sum1++;
            if(board[i][j]==black)
                sum2++;

        }
    }
    //四角
    if(board[0][0] == white)
        sum1 += 8;
    if(board[0][0] == black)
        sum2 +=8;

    if(board[0][7] == white)
        sum1 += 8;
    if(board[0][7] == black)
        sum2 +=8;

    if(board[7][0] == white)
        sum1 += 8;
    if(board[7][0] == black)
        sum2 +=8;

    if(board[7][7] == white)
        sum1 += 8;
    if(board[7][7] == black)
        sum2 +=8;
    //左
    for(i=1 ,j=0 ; i<7 ; ++i)
    {
        if(board[i][j] == white)
            sum1+=3;
        if(board[i][j] == black)
            sum2+=3;
    }
    //上
    for(i=0 ,j=1 ; j<7 ; ++j)
    {
        if(board[i][j] == white)
            sum1+=3;
        if(board[i][j] == black)
            sum2+=3;
    }
    //下
    for(i=7 ,j=1 ; j<7 ; ++j)
    {
        if(board[i][j] == white)
            sum1+=3;
        if(board[i][j] == black)
            sum2+=3;
    }
    //右
    for(i=1 ,j=7 ; i<7 ; ++i)
    {
        if(board[i][j] == white)
            sum1+=3;
        if(board[i][j] == black)
            sum2+=3;
    }
    if(side == true)
    {
        return sum1 - sum2;//white-black
    }
    else
        return sum2 - sum1;
}


void AiMode::slotComputerClick()
{
    if(b == ren)
        return;

    //随机
    //computerClick();
    //一步
    //computerClick(true);
    //两步
    computerClick(level);
    //n步极大极小
    //computerClick(level);
    result();
    update();
}



void AiMode::slotDebug()
{
    b = true;
    board[0][0] = black;
    board[0][1] = black;
    board[0][2] = black;
    board[0][3] = black;
    board[0][4] = black;
    board[0][5] = black;
    board[0][6] = black;
    board[0][7] = black;

    board[1][0] = kong;
    board[1][1] = black;
    board[1][2] = black;
    board[1][3] = black;
    board[1][4] = black;
    board[1][5] = black;
    board[1][6] = black;
    board[1][7] = kong;

    board[2][0] = kong;
    board[2][1] = kong;
    board[2][2] = white;
    board[2][3] = white;
    board[2][4] = white;
    board[2][5] = white;
    board[2][6] = white;
    board[2][7] = white;

    board[3][0] = black;
    board[3][1] = kong;
    board[3][2] = black;
    board[3][3] = black;
    board[3][4] = white;
    board[3][5] = black;
    board[3][6] = white;
    board[3][7] = white;

    board[4][0] = white;
    board[4][1] = kong;
    board[4][2] = black;
    board[4][3] = black;
    board[4][4] = kong;
    board[4][5] = black;
    board[4][6] = white;
    board[4][7] = black;

    board[5][0] = kong;
    board[5][1] = black;
    board[5][2] = black;
    board[5][3] = black;
    board[5][4] = kong;
    board[5][5] = kong;
    board[5][6] = white;
    board[5][7] = kong;

    board[6][0] = white;
    board[6][1] = kong;
    board[6][2] = black;
    board[6][3] = black;
    board[6][4] = black;
    board[6][5] = black;
    board[6][6] = white;
    board[6][7] = kong;

    board[7][0] = white;
    board[7][1] = white;
    board[7][2] = black;
    board[7][3] = black;
    board[7][4] = black;
    board[7][5] = black;
    board[7][6] = black;
    board[7][7] = black;
    update();
}

void AiMode::slotYi()
{
    computerClick(level);
    result();
    QTimer::singleShot(1000,this,SLOT(slotComputerClick()));
    update();
}

void AiMode::slotRead()
{
    mytest::slotRead();

    if(b == !ren)
    {
        QTimer::singleShot(100,this,SLOT(slotComputerClick()));
        update();

    }
}

void AiMode::slotRetract()
{
    mytest::slotRetract();
    mytest::slotRetract();
}


