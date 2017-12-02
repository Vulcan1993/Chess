#include "aimode.h"
#define debug_one 0
AiMode::AiMode(Camp xian, Camp playerCamp, QWidget *parent, int level)
    :Board(xian,playerCamp,parent)
{
    int value_tmp[32] = { 500, 300, 150, 100, 1000000, 100, 150, 300, 500, 300, 300, 100, 100, 100, 100, 100,
        500, 300, 150, 100, 1000000, 100, 150, 300, 500, 300, 300, 100, 100, 100, 100, 100 };
    for (int i = 0; i < 32; ++i)
    {
        value[i] = value_tmp[i];
    }


    int direction_tmp[4][2]={{-1,0},{1,0},{0,-1},{0,1}};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            direction[i][j] = direction_tmp[i][j];
        }
    }


    searchLevel = level;
    //设置估值函数指针
    assessTheSituation = &AiMode::staticAssess;
    //极大极小分析法
   // searchTheBestStep = MinimaxMethod;
    searchTheBestStep = &AiMode::pruning;
    xianCamp = xian;
    this->playerCamp = playerCamp;
    if(playerCamp == red)
        aiCamp = black;
    else
    {
        aiCamp = red;
    }
}

void AiMode::mousePressEvent(QMouseEvent *ev)
{
    //游戏结束后不允许再走
    if(gameOver)
        return;
    //不该人走的时候使人的点击失效
    if(alternate != playerCamp)
        return;
#if 1
    int row,col;
    //把点击的点转换为数组对应的行列值
    coordinateToRow(ev->pos(),row,col);

    //把点击的点转换成棋盘数组对应的点
    if(playerCamp == black)
        toClientRow(row,col);

    if(ev->button() == Qt::LeftButton)
    {
        if(playerClick(row,col))
        {
            //当确实走棋后判断是否分出胜负
            Camp winner;
            if(whetherTheGameIsOver(winner))
            {
                //已分出胜负
                gameOver = true;
                if(winner == red)
                {
                    QTimer::singleShot(300,this,SLOT(slotShowRedVictory()));
                }
                else
                {
                    QTimer::singleShot(300,this,SLOT(slotShowBlackVictory()));
                }

            }
            else
            {
                QTimer::singleShot(100,this,SLOT(slotComputerClick()));

            }
            update();
        }
#endif
    }
    else
    {
        undoStep();
    }

}
//静态估值,返回当前棋局的估值,当isRed为true时,返回红方的
int AiMode::staticAssess(Camp camp)
{
    int valueRed = 0;       //红方总估值
    int valueBlack = 0;     //黑方总估值
    int i;
    for(i=0 ; i<16 ; ++i)
    {
        if(chessMan[i].state != died)
            valueBlack += value[i];
    }
    for(i=16 ; i<32 ; ++i)
        if(chessMan[i].state != died)
            valueRed += value[i];
    if(camp == red)
        return valueRed - valueBlack;
    else
        return valueBlack - valueRed;
}

//获得某一阵营的所有合法走步并返回,camp为red时返回红方的所有走步
vector<Step *> *AiMode::getAllStep(Camp camp)
{
    vector<Step *> *vAllStep = new vector<Step *>;
    vector<Step *> *vStep=nullptr;
    if(camp == red)
    {
        for(int i=16 ; i<32 ; ++i)
        {
            if(chessMan[i].state != died)
            {
                vStep = generateAllSteps(i);
                //把vStep中所有的合法走步都插入到vAllStep后面
                vAllStep->insert(vAllStep->end(),vStep->begin(),vStep->end());
                delete vStep;
            }
        }
    }
    else
    {
        for(int i=0 ; i<16 ; ++i)
        {
            if(chessMan[i].state != died)
            {

                vStep = generateAllSteps(i);
                //把vStep中所有的合法走步都插入到vAllStep后面
                vAllStep->insert(vAllStep->end(),vStep->begin(),vStep->end());
                delete vStep;
            }
        }
    }
    return vAllStep;
}

//生成id棋子的所有合法走步
vector<Step *> *AiMode::generateAllSteps(int id)
{
    //vector<Step *> *vs = new
    switch(chessMan[id].type)
    {
    case redJu:
        return generateStepJu(id);
    case redMa:
        return generateStepMa(id);
    case redXiang:
        return generateRedXiang(id);
    case redShi:
        return generateRedShi(id);
    case redJiang:
        return generateRedJiang(id);
    case redPao:
        return generateStepPao(id);
    case redZu:
        return generateRedZu(id);
    case blackJu:
        return generateStepJu(id);
    case blackMa:
        return generateStepMa(id);
    case blackXiang:
        return generateBlackXiang(id);
    case blackShi:
        return generateBlackShi(id);
    case blackJiang:
        return generateBlackJiang(id);
    case blackPao:
        return generateStepPao(id);
    case blackZu:
        return generateBlackZu(id);
    }
    return NULL;
}

//(辅助函数)判断是否越界和自相残杀,返回true表示没有,
bool AiMode::checkTwoCamp(const Step *step)
{
    //判断起始点是否在越界
    if(!judgeRow(step->fromRow,step->fromCol))
        return false;
    //判断终点是否越界
    if(!judgeRow(step->toRow,step->toCol))
        return false;
    if(step->toId != -1)
    {
        //自相残杀时返回false
        if(chessMan[step->fromId].camp == chessMan[step->toId].camp)
            return false;
    }
    return true;
}

#if 1
vector<Step *> *AiMode::generateStepJu(int id)
{
    vector<Step*> * vStep = new vector<Step*>;
    int row,col;
    row = chessMan[id].row;
    col = chessMan[id].col;
    for(int i=0 ; i<4 ; ++i)
    {
        Step *step;
        //依次查找上下左右四个方向
        row = chessMan[id].row + direction[i][0];
        col = chessMan[id].col + direction[i][1];
        while(true)
        {
            //越界,结束本方向的查找
            if(!judgeRow(row,col))
                break;
            if(board[row][col] == -1)
            {
                //此时位置合法
                //构造Step加入vStep(不用再检查,一定合法)
                step = generateStep(id,row,col);
                vStep->push_back(step);
            }
            else
            {
                //此时找到这个方向的第一颗棋子,如果不是同一阵营则合法
                if(chessMan[board[row][col]].camp != chessMan[id].camp)
                {
                    step = generateStep(id,row,col);
                    vStep->push_back(step);
                }
                //找到一个子后就结束这个方向的查找
                break;
            }
            //继续查找下一个位置
            row = row + direction[i][0];
            col = col + direction[i][1];
        }
    }
    return vStep;
}

vector<Step *> *AiMode::generateStepMa(int id)
{
    vector<Step *> *vStep = new vector<Step *>;
    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col+2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col-2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col+2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col-2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row+2,chessMan[id].col+1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row+2,chessMan[id].col-1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-2,chessMan[id].col+1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-2,chessMan[id].col-1);
    vp->push_back(step);

    int i;
    for(i=0 ; i<vp->size() ; ++i)
    {
        //从前往后依次遍历
        step = vp->at(i);
        //越界或自相残杀
        if(!checkTwoCamp(step))
        {
            delete step;
            step = NULL;
            continue;
        }
        else		//暂时合法
        {
            //走步合法时存入vStep;
            if(checkStepMa(step))
            {
                vStep->push_back(step);
            }
            else
            {
                delete step;
                step = NULL;
                continue;
            }
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;
}

vector<Step *> *AiMode::generateStepPao(int id)
{
    vector<Step*> * vStep = new vector<Step*>;
    int row,col;
    row = chessMan[id].row;
    col = chessMan[id].col;
    bool first = false;		//记录是否找到第一个棋子
    Step *step;
    for(int i=0 ; i<4 ; ++i)
    {
        first = false;		//记录是否找到第一颗棋子
        //依次查找上下左右四个方向
        row = chessMan[id].row + direction[i][0];
        col = chessMan[id].col + direction[i][1];
        while(true)
        {
            //越界,结束本方向的查找
            if(!judgeRow(row,col))
                break;
            if(board[row][col] == -1)
            {
                //未找到该方向的第一颗棋子时,空白的点都可落子
                if(!first)
                {
                    //此时位置合法
                    //构造Step加入vStep(不用再检查,一定合法)
                    step = generateStep(id,row,col);
                    vStep->push_back(step);
                }
            }
            else
            {
                //当找到第一个棋子时又找到第二颗棋子且不是同一阵营的
                if(first)
                {
                    //此时找到这个方向的第一颗棋子,如果不是同一阵营则合法
                    if(chessMan[board[row][col]].camp != chessMan[id].camp)
                    {
                        step = generateStep(id,row,col);
                        vStep->push_back(step);
                    }
                    //再次找到一个子后就结束这个方向的查找
                    break;
                }
                else
                {
                    //找到第一颗棋子时记录状态
                    first = true;
                }
            }
            //继续查找下一个位置
            row = row + direction[i][0];
            col = col + direction[i][1];
        }
    }
    return vStep;
}
#endif
vector<Step *> *AiMode::generateRedXiang(int id)
{
    vector<Step *> *vStep = new vector<Step *>;
    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    step = generateStep(id,chessMan[id].row+2,chessMan[id].col+2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row+2,chessMan[id].col-2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-2,chessMan[id].col+2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-2,chessMan[id].col-2);
    vp->push_back(step);

    int i;
    for(i=0 ; i<vp->size() ; ++i)
    {
        //从前往后依次遍历
        step = vp->at(i);
        //越界或自相残杀
        if(!checkTwoCamp(step))
        {
            delete step;
            step = NULL;
            continue;
        }
        else		//暂时合法
        {
            //走步合法时存入vStep;
            if(checkRedXiang(step))
            {
                vStep->push_back(step);
            }
            else
            {
                delete step;
                step = NULL;
                continue;
            }
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;

}

vector<Step *> *AiMode::generateRedShi(int id)
{
    vector<Step *> *vStep = new vector<Step *>;
    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col+1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col-1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col+1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col-1);
    vp->push_back(step);

    int i;
    for(i=0 ; i<vp->size() ; ++i)
    {
        //从前往后依次遍历
        step = vp->at(i);
        //越界或自相残杀
        if(!checkTwoCamp(step))
        {
            delete step;
            step = NULL;
            continue;
        }
        else		//暂时合法
        {
            //走步合法时存入vStep;
            if(checkRedShi(step))
            {
                vStep->push_back(step);
            }
            else
            {
                delete step;
                step = NULL;
                continue;
            }
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;
}

vector<Step *> *AiMode::generateRedJiang(int id)
{
    vector<Step *> *vStep = new vector<Step *>;
    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row,chessMan[id].col+1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row,chessMan[id].col-1);
    vp->push_back(step);
    //将帅见面时
    step = generateStep(id,chessMan[4].row,chessMan[4].col);
    vp->push_back(step);
    //判断每一走步
    while(!vp->empty())
    {
        //从后往前依次遍历
        step = vp->back();
        //越界或自相残杀
        if(checkTwoCamp(step) == false)
        {
            delete step;
            step = NULL;
            //删除最后一个元素
            vp->pop_back();
            continue;
        }
        //走步合法时存入vStep;
        if(checkRedJiang(step))
        {
            vStep->push_back(step);
            vp->pop_back();
        }
        else		//走步不合法时销毁走步
        {
            delete step;
            step = NULL;
            //删除最后一个元素
            vp->pop_back();
            continue;
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;
}

vector<Step *> *AiMode::generateRedZu(int id)
{
    vector<Step *> *vStep = new vector<Step *>;
    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    //向上一步
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col);
    vp->push_back(step);
    //向右一步
    step = generateStep(id,chessMan[id].row,chessMan[id].col+1);
    vp->push_back(step);
    //向左一步
    step = generateStep(id,chessMan[id].row,chessMan[id].col-1);
    vp->push_back(step);
    //判断每一走步
    int i;
    for(i=0 ; i<vp->size() ; ++i)
    {
        //从前往后依次遍历
        step = vp->at(i);
        //越界或自相残杀
        if(!checkTwoCamp(step))
        {
            delete step;
            step = NULL;
            continue;
        }
        else		//暂时合法
        {
            //走步合法时存入vStep;
            if(checkRedZu(step))
            {
                vStep->push_back(step);
            }
            else
            {
                delete step;
                step = NULL;
                continue;
            }
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;
}

vector<Step *> *AiMode::generateBlackXiang(int id)
{
    vector<Step *> *vStep = new vector<Step *>;
    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    step = generateStep(id,chessMan[id].row+2,chessMan[id].col+2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row+2,chessMan[id].col-2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-2,chessMan[id].col+2);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-2,chessMan[id].col-2);
    vp->push_back(step);

    int i;
    for(i=0 ; i<vp->size() ; ++i)
    {
        //从前往后依次遍历
        step = vp->at(i);
        //越界或自相残杀
        if(!checkTwoCamp(step))
        {
            delete step;
            step = NULL;
            continue;
        }
        else		//暂时合法
        {
            //走步合法时存入vStep;
            if(checkBlackXiang(step))
            {
                vStep->push_back(step);
            }
            else
            {
                delete step;
                step = NULL;
                continue;
            }
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;
}

vector<Step *> *AiMode::generateBlackShi(int id)
{
    vector<Step *> *vStep = new vector<Step *>;

    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col+1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col-1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col+1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col-1);
    vp->push_back(step);

    int i;
    for(i=0 ; i<vp->size() ; ++i)
    {
        //从前往后依次遍历
        step = vp->at(i);
        //越界或自相残杀
        if(!checkTwoCamp(step))
        {
            delete step;
            step = NULL;
            continue;
        }
        else		//暂时合法
        {
            //走步合法时存入vStep;
            if(checkBlackShi(step))
            {
                vStep->push_back(step);
            }
            else
            {
                delete step;
                step = NULL;
                continue;
            }
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;

}

vector<Step *> *AiMode::generateBlackJiang(int id)
{
    vector<Step *> *vStep = new vector<Step *>;
    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row-1,chessMan[id].col);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row,chessMan[id].col+1);
    vp->push_back(step);
    step = generateStep(id,chessMan[id].row,chessMan[id].col-1);
    vp->push_back(step);
    //将帅见面时
    step = generateStep(id,chessMan[20].row,chessMan[20].col);
    vp->push_back(step);
    //判断每一走步
    while(!vp->empty())
    {
        //从后往前依次遍历
        step = vp->back();
        //越界或自相残杀
        if(checkTwoCamp(step) == false)
        {
            delete step;
            step = NULL;
            //删除最后一个元素
            vp->pop_back();
            continue;
        }
        //走步合法时存入vStep;
        if(checkBlackJiang(step))
        {
            vStep->push_back(step);
            vp->pop_back();
        }
        else		//走步不合法时销毁走步
        {
            delete step;
            step = NULL;
            //删除最后一个元素
            vp->pop_back();
            continue;
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;
}

vector<Step *> *AiMode::generateBlackZu(int id)
{
    vector<Step *> *vStep = new vector<Step *>;
    //先把所有可能的走步保存起来,再依次判断
    vector<Step *> *vp = new vector<Step*>;
    Step * step;
    //向下一步
    step = generateStep(id,chessMan[id].row+1,chessMan[id].col);
    vp->push_back(step);
    //向右一步
    step = generateStep(id,chessMan[id].row,chessMan[id].col+1);
    vp->push_back(step);
    //向左一步
    step = generateStep(id,chessMan[id].row,chessMan[id].col-1);
    vp->push_back(step);
    //判断每一走步
    int i;
    for(i=0 ; i<vp->size() ; ++i)
    {
        //从前往后依次遍历
        step = vp->at(i);
        //越界或自相残杀
        if(!checkTwoCamp(step))
        {
            delete step;
            step = NULL;
            continue;
        }
        else		//暂时合法
        {
            //走步合法时存入vStep;
            if(checkBlackZu(step))
            {
                vStep->push_back(step);
            }
            else
            {
                delete step;
                step = NULL;
                continue;
            }
        }
    }
    //删除临时存储走步的数组
    delete vp;
    return vStep;
}

//找电脑方的最大估值的走步
int AiMode::getMaxScore(int level)
{
    //达到要求的搜索深度,不再往下扩展,直接返回局面估值
    if(level <= 0)
        return (this->*assessTheSituation)(aiCamp);
    //Step *maxStep = NULL;		//记录最大估值的走步
    Step *tmpStep = NULL;
    int maxScore = -1000000;		//记录最大估值
    int tmpScore = -1000000;
    //获得所有合法走步
    vector<Step*> *vMaxStep = getAllStep(aiCamp);
    while(!vMaxStep->empty())
    {
        //获取vector中的最后一个元素
        tmpStep = vMaxStep->back();
        //删除最后的元素
        vMaxStep->pop_back();
        //走一步
        moveChessMan(*tmpStep);
        //估值
        tmpScore = getMinScore(level-1);
#if debug_one
        qDebug()<<"\t"<<tmpStep->fromRow<<tmpStep->fromCol<<","<<tmpStep->toRow<<tmpStep->toCol<<":"<<tmpScore;
#endif
        if(maxScore < tmpScore)
            maxScore = tmpScore;
        moveChessMan(*tmpStep,0);
        delete tmpStep;
#if 0
        if(maxScore <tmpScore)
        {
            //记录最大的估值
            maxScore = tmpScore;
            //走回去
            moveChessMan(*tmpStep,0);
        }
        else
        {
            //走回去
            moveChessMan(*tmpStep,0);
            delete tmpStep;
        }
#endif
    }
    delete vMaxStep;
    return maxScore;
}

//找电脑方的最小的估值的走步
int AiMode::getMinScore(int level)
{
    //达到要求的搜索深度,不再往下扩展,直接返回局面估值
    if(level <= 0)
        return (this->*assessTheSituation)(aiCamp);
    //Step *minStep = NULL;		//记录最大估值的走步
    Step *tmpStep = NULL;
    int minScore = 1000000;		//记录最大估值
    int tmpScore = 1000000;
    //获得所有合法走步
    vector<Step*> *vMinStep = getAllStep(playerCamp);
    while(!vMinStep->empty())
    {
        //获取vector中的最后一个元素
        tmpStep = vMinStep->back();
        //删除最后的元素
        vMinStep->pop_back();
        //走一步
        moveChessMan(*tmpStep);
        //估值
#if debug_one
        qDebug()<<tmpStep->fromRow<<tmpStep->fromCol<<","<<tmpStep->toRow<<tmpStep->toCol;
#endif
        tmpScore = getMaxScore(level-1);
#if debug_one
        qDebug()<<"tmpScore:"<<tmpScore;
#endif
        if(minScore > tmpScore)
            minScore = tmpScore;
        moveChessMan(*tmpStep,0);
        delete tmpStep;
#if 0
        if(minScore > tmpScore)
        {
            //记录最大的估值
            minScore = tmpScore;
            //走回去
            moveChessMan(*tmpStep,0);
        }
        else
        {
            //走回去
            moveChessMan(*tmpStep,0);
            delete tmpStep;
        }
#endif
#if debug_one
        qDebug()<<"*******minScore"<<minScore<<",tmpScore"<<tmpScore;
#endif
    }
    delete vMinStep;
    return minScore;
}

//极大极小分析法
Step* AiMode::MinimaxMethod()
{
    Step *maxStep = NULL;		//记录最大估值的走步
    Step *tmpStep = NULL;
    int maxScore = -1000000;		//记录最大估值
    int tmpScore = -1000000;
    //获得所有合法走步
    vector<Step*> *vMaxStep = getAllStep(aiCamp);
    while(!vMaxStep->empty())
    {
        //获取vector中的最后一个元素
        tmpStep = vMaxStep->back();
        //删除最后的元素
        vMaxStep->pop_back();
        //走一步
        moveChessMan(*tmpStep);
#if debug_one
        qDebug()<<"***********Max  Begin****"<<tmpStep->fromRow<<tmpStep->fromCol<<","
               <<tmpStep->toRow<<tmpStep->toCol;
#endif
        //估值
        tmpScore = getMinScore(searchLevel-1);

        if(maxScore <tmpScore)
        {
            //找到更好的走步,把之前保存的走步删除
            if(maxStep != NULL)
                delete maxStep;
            //记录得出最大估值的走步
            maxStep = tmpStep;
            //记录最大的估值
            maxScore = tmpScore;
            //走回去
            moveChessMan(*tmpStep,0);
        }
        else
        {
            //走回去
            moveChessMan(*tmpStep,0);
            delete tmpStep;
        }
#if debug_one
        qDebug()<<"*********Max  End*******MaxScore"<<maxScore<<",tmpScore"<<tmpScore;
#endif
    }
    delete vMaxStep;
    return maxStep;
}

//β剪枝(电脑走时候剪,根据目前人目前找到的最小值),max为目前人走的,给电脑的最小的值(在一堆最大值里找的最小值)
//max为上确界
int AiMode::pruningMaxScore(int level, int max)
{
    //达到要求的搜索深度,不再往下扩展,直接返回局面估值
    if(level <= 0)
        return (this->*assessTheSituation)(aiCamp);
    //Step *maxStep = NULL;		//记录最大估值的走步
    Step *tmpStep = NULL;
    int maxScore = -1000000;		//记录最大估值
    int tmpScore = -1000000;
    //获得所有合法走步
    vector<Step*> *vMaxStep = getAllStep(aiCamp);
    while(!vMaxStep->empty())
    {
        //获取vector中的最后一个元素
        tmpStep = vMaxStep->back();
        //删除最后的元素
        vMaxStep->pop_back();
        //走一步
        moveChessMan(*tmpStep);
        //估值
        //tmpScore = getMinScore(level-1);
        tmpScore = pruningMinScore(level-1,maxScore);
        //β剪枝
        if(tmpScore >= max)
        {
            //把棋子移回去
            moveChessMan(*tmpStep,0);
            delete tmpStep;
            //把剩余的节点删除,防止内存泄漏
            for(int i=0 ; i<vMaxStep->size() ; ++i)
            {
                delete vMaxStep->at(i);
            }
            delete vMaxStep;
            //返回值
            return tmpScore;
        }
#if debug_one
        qDebug()<<"\t"<<tmpStep->fromRow<<tmpStep->fromCol<<","<<tmpStep->toRow<<tmpStep->toCol<<":"<<tmpScore;
#endif
        if(maxScore < tmpScore)
            maxScore = tmpScore;
        moveChessMan(*tmpStep,0);
        delete tmpStep;
#if 0
        if(maxScore <tmpScore)
        {
            //记录最大的估值
            maxScore = tmpScore;
            //走回去
            moveChessMan(*tmpStep,0);
        }
        else
        {
            //走回去
            moveChessMan(*tmpStep,0);
            delete tmpStep;
        }
#endif
    }
    delete vMaxStep;
    return maxScore;

}

//α剪枝,min为目前电脑走的,给电脑的最大值(在一堆最小值里找最大值)
//min为下确界
int AiMode::pruningMinScore(int level, int min)
{
    //达到要求的搜索深度,不再往下扩展,直接返回局面估值
    if(level <= 0)
        return (this->*assessTheSituation)(aiCamp);
    //Step *minStep = NULL;		//记录最大估值的走步
    Step *tmpStep = NULL;
    int minScore = 1000000;		//记录最大估值
    int tmpScore = 1000000;
    //获得所有合法走步
    vector<Step*> *vMinStep = getAllStep(playerCamp);
    while(!vMinStep->empty())
    {
        //获取vector中的最后一个元素
        tmpStep = vMinStep->back();
        //删除最后的元素
        vMinStep->pop_back();
        //走一步
        moveChessMan(*tmpStep);
        //估值
#if debug_one
        qDebug()<<tmpStep->fromRow<<tmpStep->fromCol<<","<<tmpStep->toRow<<tmpStep->toCol;
#endif
        tmpScore = pruningMaxScore(level-1,minScore);
        //α剪枝
        if(tmpScore <= min)
        {
            //把棋子移回去
            moveChessMan(*tmpStep,0);
            delete tmpStep;
            //把剩余的节点删除,防止内存泄漏
            for(int i=0 ; i<vMinStep->size() ; ++i)
            {
                delete vMinStep->at(i);
            }
            delete vMinStep;
            //返回极小值
            return tmpScore;
        }
      //  tmpScore = getMaxScore(level-1);
#if debug_one
        qDebug()<<"tmpScore:"<<tmpScore;
#endif
        if(minScore > tmpScore)
            minScore = tmpScore;
        moveChessMan(*tmpStep,0);
        delete tmpStep;
#if 0
        if(minScore > tmpScore)
        {
            //记录最大的估值
            minScore = tmpScore;
            //走回去
            moveChessMan(*tmpStep,0);
        }
        else
        {
            //走回去
            moveChessMan(*tmpStep,0);
            delete tmpStep;
        }
#endif
#if debug_one
        qDebug()<<"*******minScore"<<minScore<<",tmpScore"<<tmpScore;
#endif
    }
    delete vMinStep;
    return minScore;

}

Step *AiMode::pruning()
{
    Step *maxStep = NULL;		//记录最大估值的走步
    Step *tmpStep = NULL;
    int maxScore = -1000000;		//记录最大估值
    int tmpScore = -1000000;
    //获得所有合法走步
    vector<Step*> *vMaxStep = getAllStep(aiCamp);
    while(!vMaxStep->empty())
    {
        //获取vector中的最后一个元素
        tmpStep = vMaxStep->back();
        //删除最后的元素
        vMaxStep->pop_back();
        //走一步
        moveChessMan(*tmpStep);
#if debug_one
        qDebug()<<"***********Max  Begin****"<<tmpStep->fromRow<<tmpStep->fromCol<<","
               <<tmpStep->toRow<<tmpStep->toCol;
#endif
        //估值
        tmpScore = pruningMinScore(searchLevel-1,maxScore);

        if(maxScore <tmpScore)
        {
            //找到更好的走步,把之前保存的走步删除
            if(maxStep != NULL)
                delete maxStep;
            //记录得出最大估值的走步
            maxStep = tmpStep;
            //记录最大的估值
            maxScore = tmpScore;
            //走回去
            moveChessMan(*tmpStep,0);
        }
        else
        {
            //走回去
            moveChessMan(*tmpStep,0);
            delete tmpStep;
        }
#if debug_one
        qDebug()<<"*********Max  End*******MaxScore"<<maxScore<<",tmpScore"<<tmpScore;
#endif
    }
    delete vMaxStep;
    return maxStep;

}

void AiMode::slotDebug()
{
    vector<Step*> *vStep1 = generateStepPao(9);
#if 0
    vector<Step*> *vStep1 = getAllStep(black);
#endif
    qDebug()<<"***************slotDebug() Begin**************";
    for(Step* item : *vStep1)
    {
        qDebug()<<(item->fromRow)<<(item->fromCol)<<(item->toRow)<<(item->toCol);
    }
    qDebug()<<"***************slotDebug() End**************";
    for(Step* item : *vStep1)
    {
        delete item;
    }
    delete vStep1;
}

void AiMode::slotComputerClick()
{
    if(alternate != aiCamp)
        return;
    Step *step = (this->*searchTheBestStep)();
    if(step!=NULL)
    {
        moveChessMan(*step);
        finallyMoveChessMan = step->fromId;
        recordStep(step);
        //当确实走棋后判断是否分出胜负
        Camp winner;
        if(whetherTheGameIsOver(winner))
        {
            //已分出胜负
            gameOver = true;
            if(winner == red)
            {
                QTimer::singleShot(300,this,SLOT(slotShowRedVictory()));
            }
            else
            {
                QTimer::singleShot(300,this,SLOT(slotShowBlackVictory()));
            }

        }
    }
    else
        qDebug()<<"step == NULL  true";
    //让另一方走
    alternateNextPlayer();
    update();
}

void AiMode::restart()
{
    //重新设置棋子和棋盘位置
    initChess();
    //重新设置游戏结束标志
    gameOver = false;
    //销毁vStep里所有的走步
    for(Step* item : vStep)
        delete item;
    //清空悔棋信息数组
    vStep.clear();
    //设置谁先走
    alternate  = xianCamp;
    //设置为没有最后移动的棋子
    finallyMoveChessMan = -1;
    if(alternate == aiCamp)
        QTimer::singleShot(1000,this,SLOT(slotComputerClick()));
    update();
}

void AiMode::undoStep()
{
    Board::undoStep();
    Board::undoStep();
}

void AiMode::slotStart()
{

    if(aiCamp == alternate)
        QTimer::singleShot(100,this,SLOT(slotComputerClick()));

    Board::slotStart();
}
