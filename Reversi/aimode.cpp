#include"aimode.h"
#include<QDebug>
#include<windows.h>
#define debug_one 0
#define debug_two 0


AiMode::AiMode(bool ren, int level)
{
    //确定人走哪方的子
    this->m_ren = ren;
    if(ren == true)
        m_label2->setText("电脑执黑");
    else
        m_label2->setText("电脑执白");
    //确搜索深度
    this->m_level = level;
    //true表示白子
    if(ren == true)
        slot_computerClick();
    //连接,实现一步对战两步
    connect(m_btn6,SIGNAL(clicked(bool)),this,SLOT(slot_one()));
}

#if 1
int AiMode::result()
{
    if(mytest::result() == 1 && m_winner == false)
    {
        //如果不该人走了,则让电脑走
        if(m_b != m_ren)
            QTimer::singleShot(1000,this,SLOT( slot_computerClick()));
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
    if(m_winner)
        return;
    if(m_b == !m_ren)
        return;
    int i,j;
    j=ev->pos().x()/m_step;
    i=ev->pos().y()/m_step;
    QPoint p(i,j);
    //不在棋盘内
    if(i<0||i>8||j<0||j>8)
        return;


    if(ev->button()==Qt::LeftButton)
    {
        vector<QPoint> v;
        onepointexec(v,p,m_b);
        if(0 != v.size())
        {
            //存储这一步
            record(v,p,m_b);
            if(m_b==true)
                m_board[i][j]=white;
            else
                m_board[i][j]=black;
            for(QPoint a:v)
            {
                if(m_b==true)
                    m_board[a.x()][a.y()]=white;
                else
                    m_board[a.x()][a.y()]=black;
            }
            update();
        //该另一方走了
            //清除之前的提示
            clearEarning();
            result();
           QTimer::singleShot(1000,this,SLOT( slot_computerClick()));
            //Sleep(100);
          //  computerClick();
         //   b = !b;

        }
    }//leftbutton

    if(ev->button()==Qt::RightButton)
        m_board[i][j]=kong;
    update();

}




//走回去
void AiMode::computerMove(vector<QPoint> &vp, vector<vector<QPoint> > &vv, bool state,int i, int)
{
    //消除白方的走步
    if(state == true)
    {
        m_board[vp[i].x()][vp[i].y()] = kong;
        for(QPoint p : vv[i])
        {
            m_board[p.x()][p.y()] = black;
        }

    }
    //消除黑方的走步
    else
    {
        m_board[vp[i].x()][vp[i].y()] = kong;
        for(QPoint p : vv[i])
        {
            m_board[p.x()][p.y()] = white;
        }
    }
}
//走棋
void AiMode::computerMove(vector<QPoint> &vp,vector<vector<QPoint>> &vv,bool state,int i)
{
    if(state == true)
    {
        //下这个点
        m_board[vp[i].x()][vp[i].y()] = white;
        //翻子
        for(QPoint p : vv[i])
        {
            m_board[p.x()][p.y()] = white;
        }

    }
    else
    {
        m_board[vp[i].x()][vp[i].y()] = black;
        for(QPoint p : vv[i])
        {
            m_board[p.x()][p.y()] = black;
        }
    }
}



//随机
void AiMode::computerClick()
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;

    earn(vp,vv,m_b,false);
    if(vp.empty())
        return;
    int i = qrand()%vp.size();

    computerMove(vp,vv,m_b,i);

        record(vv[i],vp[i],m_b);

}

//一步人工智能
void AiMode::computerClick(bool)
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,m_b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,m_b,i);
        //估值
        scoreTmp = score(true);
        //找到最大估值,最好的一步棋
        if(maxScore < scoreTmp)
        {
            maxScore = scoreTmp;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,m_b,i,0);
    }
    //走最好的一步
    if(maxStep!= -1)
    {
        computerMove(vp,vv,m_b,maxStep);
        record(vv[maxStep],vp[maxStep],m_b);
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
    earn(vp,vv,m_b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;
    int scoreTmp=-10000;
    int minScore = 10000;

    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设电脑走一步,翻子
        computerMove(vp,vv,m_b,i);

        vector<QPoint> vpr;
        vector<vector<QPoint>> vvr;
        earn(vpr,vvr,!m_b,false);
        minScore = 10000;
        //int minStep = -1;
        for(int i=0 ; i<vpr.size() ; ++i)
        {
            //假设人走一步,翻子
            computerMove(vpr,vvr,!m_b,i);
            //估值
            scoreTmp = score(false);
            //找到最小值，对电脑最不利的局面
            if(minScore > scoreTmp)
            {
                minScore = scoreTmp;
            }
            //人再走回去
            computerMove(vpr,vvr,!m_b,i,0);
        }
        if(maxScore < minScore)
        {
            maxScore = minScore;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,m_b,i,0);
    }
    //走最好的一步
    if(maxStep != -1)
    {
        computerMove(vp,vv,m_b,maxStep);
        record(vv[maxStep],vp[maxStep],m_b);
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
    earn(vp,vv,m_b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,m_b,i);
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
        computerMove(vp,vv,m_b,i,0);
    }
    return maxScore;
}

int AiMode::getMinScore(int level)
{
    if(level <= 0)
        return score(true);
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,!m_b,false);
    //记录最大估值
    int minScore = 10000;
    //记录最好的一步棋的下标

    int scoreTmp=10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,!m_b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMaxScore(level - 1);

        //找到最大估值,最好的一步棋
        if(minScore > scoreTmp)
        {
            minScore = scoreTmp;
        }
        //走回去
        computerMove(vp,vv,!m_b,i,0);
    }
    return minScore;
}

void AiMode::computerClick(int level)
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,m_b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,m_b,i);
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
        computerMove(vp,vv,m_b,i,0);
    }
    //走最好的一步
    if(maxStep!= -1)
    {
        computerMove(vp,vv,m_b,maxStep);
        record(vv[maxStep],vp[maxStep],m_b);
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
    earn(vp,vv,m_b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,m_b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMinScore(level - 1,maxScore);
        //β剪枝
        if(scoreTmp > max)
        {
            //走回去
            computerMove(vp,vv,m_b,i,0);
            return scoreTmp;
        }
        //找到最大估值,最好的一步棋
        if(maxScore < scoreTmp)
        {
            maxScore = scoreTmp;
            maxStep = i;
        }
        //走回去
        computerMove(vp,vv,m_b,i,0);
    }
    return maxScore;
}

int AiMode::getMinScore(int level,int min)
{
    if(level <= 0)
        return score(true);
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,!m_b,false);
    //记录最大估值
    int minScore = 10000;
    //记录最好的一步棋的下标

    int scoreTmp=10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,!m_b,i);
        //估值
        //scoreTmp = score(true);
        scoreTmp = getMaxScore(level - 1,minScore);
        //α剪枝
        if(scoreTmp < min)
        {
            //走回去
            computerMove(vp,vv,!m_b,i,0);
            return scoreTmp;
        }
        //找到最大估值,最好的一步棋
        if(minScore > scoreTmp)
        {
            minScore = scoreTmp;
        }
        //走回去
        computerMove(vp,vv,!m_b,i,0);
    }
    return minScore;
}



void AiMode::computerClick(char level)
{
    vector<QPoint> vp;
    vector<vector<QPoint>> vv;
    earn(vp,vv,m_b,false);
    //记录最大估值
    int maxScore = -10000;
    //记录最好的一步棋的下标
    int maxStep = -1;

    int scoreTmp=-10000;
    for(int i=0 ; i<vp.size() ; ++i)
    {
        //假设走一步,翻子
        computerMove(vp,vv,m_b,i);
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
        computerMove(vp,vv,m_b,i,0);
    }
    //走最好的一步
    if(maxStep!= -1)
    {
        computerMove(vp,vv,m_b,maxStep);
        record(vv[maxStep],vp[maxStep],m_b);
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
            if(m_board[i][j]==white)
                sum1++;
            if(m_board[i][j]==black)
                sum2++;

        }
    }
    //四角
    if(m_board[0][0] == white)
        sum1 += 8;
    if(m_board[0][0] == black)
        sum2 +=8;

    if(m_board[0][7] == white)
        sum1 += 8;
    if(m_board[0][7] == black)
        sum2 +=8;

    if(m_board[7][0] == white)
        sum1 += 8;
    if(m_board[7][0] == black)
        sum2 +=8;

    if(m_board[7][7] == white)
        sum1 += 8;
    if(m_board[7][7] == black)
        sum2 +=8;
    //左
    for(i=1 ,j=0 ; i<7 ; ++i)
    {
        if(m_board[i][j] == white)
            sum1+=3;
        if(m_board[i][j] == black)
            sum2+=3;
    }
    //上
    for(i=0 ,j=1 ; j<7 ; ++j)
    {
        if(m_board[i][j] == white)
            sum1+=3;
        if(m_board[i][j] == black)
            sum2+=3;
    }
    //下
    for(i=7 ,j=1 ; j<7 ; ++j)
    {
        if(m_board[i][j] == white)
            sum1+=3;
        if(m_board[i][j] == black)
            sum2+=3;
    }
    //右
    for(i=1 ,j=7 ; i<7 ; ++i)
    {
        if(m_board[i][j] == white)
            sum1+=3;
        if(m_board[i][j] == black)
            sum2+=3;
    }
    if(side == true)
    {
        return sum1 - sum2;//white-black
    }
    else
        return sum2 - sum1;
}


void AiMode::slot_computerClick()
{
    if(m_b == m_ren)
        return;

    //随机
    //computerClick();
    //一步
    //computerClick(true);
    //两步
    computerClick(m_level);
    //n步极大极小
    //computerClick(level);
    result();
    update();
}



void AiMode::slot_debug()
{
    m_b = true;
    m_board[0][0] = black;
    m_board[0][1] = black;
    m_board[0][2] = black;
    m_board[0][3] = black;
    m_board[0][4] = black;
    m_board[0][5] = black;
    m_board[0][6] = black;
    m_board[0][7] = black;

    m_board[1][0] = kong;
    m_board[1][1] = black;
    m_board[1][2] = black;
    m_board[1][3] = black;
    m_board[1][4] = black;
    m_board[1][5] = black;
    m_board[1][6] = black;
    m_board[1][7] = kong;

    m_board[2][0] = kong;
    m_board[2][1] = kong;
    m_board[2][2] = white;
    m_board[2][3] = white;
    m_board[2][4] = white;
    m_board[2][5] = white;
    m_board[2][6] = white;
    m_board[2][7] = white;

    m_board[3][0] = black;
    m_board[3][1] = kong;
    m_board[3][2] = black;
    m_board[3][3] = black;
    m_board[3][4] = white;
    m_board[3][5] = black;
    m_board[3][6] = white;
    m_board[3][7] = white;

    m_board[4][0] = white;
    m_board[4][1] = kong;
    m_board[4][2] = black;
    m_board[4][3] = black;
    m_board[4][4] = kong;
    m_board[4][5] = black;
    m_board[4][6] = white;
    m_board[4][7] = black;

    m_board[5][0] = kong;
    m_board[5][1] = black;
    m_board[5][2] = black;
    m_board[5][3] = black;
    m_board[5][4] = kong;
    m_board[5][5] = kong;
    m_board[5][6] = white;
    m_board[5][7] = kong;

    m_board[6][0] = white;
    m_board[6][1] = kong;
    m_board[6][2] = black;
    m_board[6][3] = black;
    m_board[6][4] = black;
    m_board[6][5] = black;
    m_board[6][6] = white;
    m_board[6][7] = kong;

    m_board[7][0] = white;
    m_board[7][1] = white;
    m_board[7][2] = black;
    m_board[7][3] = black;
    m_board[7][4] = black;
    m_board[7][5] = black;
    m_board[7][6] = black;
    m_board[7][7] = black;
    update();
}

void AiMode::slot_one()
{
    computerClick(m_level);
    result();
    QTimer::singleShot(1000,this,SLOT(slot_computerClick()));
    update();
}

void AiMode::slot_read()
{
    mytest::slot_read();

    if(m_b == !m_ren)
    {
        QTimer::singleShot(100,this,SLOT(slot_computerClick()));
        update();

    }
}

void AiMode::slot_retract()
{
    mytest::slot_retract();
    mytest::slot_retract();
}


