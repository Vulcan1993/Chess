#include"board.h"
#include<QHBoxLayout>
#include<QDebug>
#include<QPushButton>
Board::Board(Camp xian, Camp playerCamp, QWidget *parent)
    :QWidget(parent)
{
    initChess();

    //initImage(false,2);
    //设置哪方先走
    m_alternate = xian;
    m_xianCamp = xian;
    this->m_playerCamp = playerCamp;
    initImage(playerCamp,2);

    //等50毫秒再发送信号,因为此时信号和槽还未连接,如果程序在50毫秒内没有连接
    //则不会显示显示这条消息
    QTimer::singleShot(50,this,SLOT(slotInitialize()));
#if 0
    if(playerCamp == red)
        initImage(true,1);
    else
        initImage(false,1);
#endif
    //重设格子宽度和高度,
    m_boardGridWidth = width()/10;
    m_boardGridHeigh = height()/11;
    m_pixmapBacks.load(":/board/boardwq");

    resize(500,600);
}


void Board::paintEvent(QPaintEvent *)
{
    QPixmap pixmap = m_pixmapBacks;
    //加载棋盘图片
   // pixmap.load(":/board/board");
    QPainter p;

#if 0
    //设置反锯齿
    //p.setRenderHint(QPainter::Antialiasing);
    p.begin(&pixmap);

    for(int k=0 ; k<10;++k)
    {
        for(int j=0 ; j<9;++j)
        {
            if(board[k][j] == -1)
                continue;
            int i=board[k][j];
            QPoint point( 100*j + dr,100*k + dr );
            //注意顺序,应先画蓝框,再画红框,否则当选择时可能蓝框覆盖红框,(悔棋时可能出现)
            //如果当前棋子时走的最后一颗棋子,则画一个蓝色的方框,
            if(finallyMoveChessMan == i)
                p.drawImage(point,imageFinallyMove.scaled(scaleSize));
            //如果当前棋子时选中的棋子,则画一个红色的方框
            if(selectedChessMan == i)
                 p.drawImage(point,imageSelectedBorder.scaled(scaleSize));
            //画棋子
            p.drawImage(point,images[chessMan[i].type].scaled(scaleSize));

        }
    }
    p.end();
#endif
    paintChessMan( p,pixmap);
    p.begin(this);
    //  p.drawPixmap(0,0,pixmap);
    //缩放,使其适应窗口
    QPixmap fitPixmap = pixmap.scaled(width(),height(), Qt::IgnoreAspectRatio);

#if 1
    if(m_playerCamp == black)
    {
        //旋转
        //求中心点
        QPointF center1(width()/2.0,height()/2.0);
        //重置原点
        p.translate(center1);
        //旋转
        p.rotate(180);
        //把原点还原
        p.translate(-center1);
    }
#endif

     p.drawPixmap(0,0,fitPixmap);
     p.end();
}

void Board::mousePressEvent(QMouseEvent *ev)
{
    //游戏结束后不允许再走
    if(m_gameOver)
        return;
#if 1
    int row,col;
    //把点击的点转换为数组对应的行列值
    coordinateToRow(ev->pos(),row,col);
    //把点击的点转换成棋盘数组对应的点
    if(m_playerCamp == black)
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
                m_gameOver = true;
                if(winner == red)
                {
                    QTimer::singleShot(300,this,SLOT(slotShowRedVictory()));
                }
                else
                {
                    QTimer::singleShot(300,this,SLOT(slotShowBlackVictory()));
                }
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

void Board::resizeEvent(QResizeEvent *)
{

    m_boardGridWidth = width()/10;
    m_boardGridHeigh = height()/11;

}
//把屏幕坐标转换成对应的数组的行列值
void Board::coordinateToRow(QPoint point, int &row, int &col)
{
    if(point.x()<m_boardGridWidth/2 || point.x()> width()-m_boardGridWidth/2
            || point.y() < m_boardGridHeigh/2 || point.y() > height()-m_boardGridHeigh/2)
    {
        col = -1;
        row = -1;
    }
    else
    {
        col = (point.x()-m_boardGridWidth/2)/m_boardGridWidth ;
        row = (point.y()-m_boardGridHeigh/2)/m_boardGridHeigh ;
    }   
}
//判断行列值是否合法,合法返回true,不合法返回false;
bool Board::judgeRow(const int &row, const int &col)
{
    if(row>=0 && row<=9 && col>=0 && col <=8)
        return true;
    return false;
}

//移动棋子(从(fromRow,fromCol)移动到(toRow,toCol));假设位置都合法,走的这一步合法
void Board::moveChessMan(const Step &step)
{
    int fromId = step.fromId;
    int toId = step.toId;          //获取(toRow,toCol)位置的棋子id;
    //设置棋子数组中棋子的新位置
    m_chessMan[fromId].row = step.toRow;
    m_chessMan[fromId].col = step.toCol;
    
    if(toId != -1)                              //该点有棋子
    {
        m_chessMan[toId].state = died;            //棋子出局
    }
    //更新棋盘中的记录
    //更新棋盘中的记录
    m_board[step.fromRow][step.fromCol] = -1;               //设置该位置无棋子
    m_board[step.toRow][step.toCol] = fromId;

}
//把棋子移回去(重载函数),出局的棋子最后保存的就是它最后呆的位置
void Board::moveChessMan(const Step &step,int)
{
    int id = -1;
    id = step.toId;
    //有棋子出局则恢复,出局的棋子保存的行列值即是他最后的位置
    if(step.toId != -1)
    {
        //恢复
        m_chessMan[id].state = alive;
        m_board[step.toRow][step.toCol] = id;
    }
    else
    {
        m_board[step.toRow][step.toCol] = -1;
    }

    //把fromId的棋子移回去
    id = step.fromId;
    m_chessMan[id].row = step.fromRow;
    m_chessMan[id].col = step.fromCol;
    m_board[step.fromRow][step.fromCol] = id;

}

//记录走棋信息
void Board::recordStep(Step *step)
{
    m_vStep.push_back(step);
}

//悔棋
void Board::undoStep()
{
    if(m_vStep.empty())
        return;
    //当游戏结束时悔棋,应把游戏状态置为未结束
    if(m_gameOver == true)
        m_gameOver = false;
    Step *stepTmp = m_vStep.back();

    moveChessMan(*stepTmp,0);	//走回去
    m_alternate = m_chessMan[stepTmp->fromId].camp;
    m_vStep.pop_back();    		//删除这一步的记录信息

    delete stepTmp;				//删除这一步
    update();
}

void Board::slotShowRedVictory()
{
    QMessageBox::information(this,"游戏结束","红方胜!",
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    emit changeState("当前状态 : 游戏结束 , 红方胜");
}

void Board::slotShowBlackVictory()
{
    QMessageBox::information(this,"游戏结束","黑方胜!",
                             QMessageBox::Ok,
                             QMessageBox::Ok);
    emit changeState("当前状态 : 游戏结束 , 黑方胜");
}

void Board::slotRedXian()
{
    m_xianCamp = red;
    m_playerCamp = red;
    initImage(m_playerCamp,2);
    restart();
    update();
}

void Board::slotBlackXian()
{
    m_xianCamp = black;
    m_playerCamp = black;
    initImage(m_playerCamp,2);
    restart();
    update();
}

void Board::slotInitialize()
{
    emit changeState("当前状态 : 未开始 ");
}

void Board::toClientRow(int &row, int &col)
{
    row = 9-row;
    col = 8-col;
}

//生成走步,假设selectedChessMan,row,col都合法
Step *Board::generateStep(int selectedChessMan, int row, int col)
{
    Step *step;
    step = new Step;
    step->fromRow = m_chessMan[selectedChessMan].row;
    step->fromCol = m_chessMan[selectedChessMan].col;
    step->fromId = selectedChessMan;
    step->toRow = row;
    step->toCol = col;
    step->toId = m_board[row][col];
    return step;
}

//设置成另一方走棋
void Board::alternateNextPlayer()
{
    //让另一方走
    if(m_alternate == black)
    {
        m_alternate = red;
        emit changeState("当前状态 : 红方下");
    }
    else
    {
        m_alternate = black;
        emit changeState("当前状态 : 黑方下");
    }
}

//处理点击的点,如果确实走棋了则返回true,
bool Board::playerClick(int row, int col)
{
    //如果不合法则退出
    if(!judgeRow(row,col))
        return false;
    //点击的位置有棋子
    if(m_board[row][col] != -1)
    {
        if(m_alternate == m_chessMan[m_board[row][col]].camp)
        {
            //未选择棋子则选中棋子
            if(m_selectedChessMan == -1)
            {
                m_selectedChessMan = m_board[row][col];
            }
            else		//此时应该换选择
            {
                //是同一阵营的棋子时要换选择
                m_selectedChessMan = m_board[row][col];
            }
            //换选择后要刷新
            update();
        }
        else
        {
            //如果已选择本阵营的棋子,再选择其他阵营的棋子时,表示要吃子
            if(m_selectedChessMan != -1)
            {
                //生成走步
                m_step = generateStep(m_selectedChessMan,row,col);
                //判断走步是否合法
                if(checkStep(m_step) == false)
                {
                    delete m_step;
                    m_step = NULL;
                    return false;
                }
                moveChessMan(*m_step);
                m_finallyMoveChessMan = m_selectedChessMan;
                //重置为未选择棋子的状态
                m_selectedChessMan = -1;
                recordStep(m_step);
                m_step = NULL;
                //让另一方走
                alternateNextPlayer();
                return true;
            }
        }
    }
    else		//走空地方
    {
        //如果之前已选择棋子,则应走棋
        if(m_selectedChessMan != -1)
        {
            //生成走步
            m_step = generateStep(m_selectedChessMan,row,col);
            //判断走步是否合法
            if(checkStep(m_step) == false)
            {
                delete m_step;
                m_step = NULL;
                return false;
            }
            moveChessMan(*m_step);
            m_finallyMoveChessMan = m_selectedChessMan;
            //重置为未选择棋子的状态
            m_selectedChessMan = -1;
            recordStep(m_step);
            m_step = NULL;
            //让另一方走
            alternateNextPlayer();

            return true;
        }
    }
    return false;
}

//判断游戏是否结束,返回true表示结束,winner里保存胜者的阵营
bool Board::whetherTheGameIsOver(Camp &winner)
{
    //黑方的将出局
    if(m_chessMan[4].state == died)
    {
        winner = red;
        return true;
    }
    //红方的将出局
    if(m_chessMan[20].state == died)
    {
        winner = black;
        return true;
    }
    return false;
}

//判断棋子是否能走,如果能走则返回true,否则返回false
bool Board::checkStep(const Step *step)
{
    //同一个点,因为一个id仅能对应一个子,id相等,说明是同一个子
    if(step->fromId == step->toId)
        return false;

    switch(m_chessMan[step->fromId].type)
    {
    case redJu:
        return checkStepJu(step);
    case redMa:
        return checkStepMa(step);
    case redXiang:
        return checkRedXiang(step);
    case redShi:
        return checkRedShi(step);
    case redJiang:
        return checkRedJiang(step);
    case redPao:
        return checkStepPao(step);
    case redZu:
        return checkRedZu(step);
    case blackJu:
        return checkStepJu(step);
    case blackMa:
        return checkStepMa(step);
    case blackXiang:
        return checkBlackXiang(step);
    case blackShi:
        return checkBlackShi(step);
    case blackJiang:
        return checkBlackJiang(step);
    case blackPao:
        return checkStepPao(step);
    case blackZu:
        return checkBlackZu(step);
    }
    return false;
}

//计算两点之间的棋子个数并返回,-1表示两点不在一条线上
int Board::countChessMan(const Step *step)
{
    int sum=0;
    //行号相等表示在同一行
    if(step->fromRow == step->toRow)
    {
        int min = step->fromCol;
        int max = step->toCol;
        //调整大小,使min小于max
        if(min>max)
        {
            min = min ^ max;
            max = min ^ max;
            min = min ^ max;
        }
        for(min++ ; min<max ; ++min)
        {
            if(m_board[step->fromRow][min] != -1)
                ++sum;
        }
    }
    else
    {
        //列号相等表示在同一列
        if(step->fromCol == step->toCol)
        {
            int min = step->fromRow;
            int max = step->toRow;
            //调整大小,使min小于max
            if(min>max)
            {
                min = min ^ max;
                max = min ^ max;
                min = min ^ max;
            }
            for(min++ ; min<max ; ++min)
            {
                if(m_board[min][step->fromCol] != -1)
                    ++sum;
            }
        }
        else
            return -1;
    }
    return sum;
}
//画棋子
void Board::paintChessMan(QPainter &painter, QPixmap &pixmap)
{
    //设置反锯齿
    //p.setRenderHint(QPainter::Antialiasing);
    painter.begin(&pixmap);

    //按棋子信息画棋子
    for(int i=0 ; i<32 ; ++i)
    {
       if(m_chessMan[i].state == died)
           continue;
       QPoint point(100*m_chessMan[i].col + m_dr , 100*m_chessMan[i].row + m_dr);
       //注意顺序,应先画蓝框,再画红框,否则当选择时可能蓝框覆盖红框,(悔棋时可能出现)
       //如果当前棋子时走的最后一颗棋子,则画一个蓝色的方框,
       if(m_finallyMoveChessMan == i)
           painter.drawImage(point,m_imageFinallyMove.scaled(m_scaleSize));
       //如果当前棋子是选中的棋子,则画一个红色的方框
       if(m_selectedChessMan == i)
           painter.drawImage(point,m_imageSelectedBorder.scaled(m_scaleSize));
       //画棋子
       painter.drawImage(point,m_images[m_chessMan[i].type].scaled(m_scaleSize));
    }
#if 0
    //按棋盘信息画棋子
    for(int k=0 ; k<10;++k)
    {
        for(int j=0 ; j<9;++j)
        {
            if(board[k][j] == -1)
                continue;
            int i=board[k][j];
            QPoint point( 100*j + dr,100*k + dr );
            //注意顺序,应先画蓝框,再画红框,否则当选择时可能蓝框覆盖红框,(悔棋时可能出现)
            //如果当前棋子时走的最后一颗棋子,则画一个蓝色的方框,
            if(finallyMoveChessMan == i)
                painter.drawImage(point,imageFinallyMove.scaled(scaleSize));
            //如果当前棋子时选中的棋子,则画一个红色的方框
            if(selectedChessMan == i)
                 painter.drawImage(point,imageSelectedBorder.scaled(scaleSize));
            //画棋子
            painter.drawImage(point,images[chessMan[i].type].scaled(scaleSize));

        }
    }
#endif
    painter.end();
}

bool Board::checkStepJu(const Step *step)
{
   int num = countChessMan(step);
   if(num != 0)
       return false;
   return true;
}

bool Board::checkStepMa(const Step *step)
{
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;	//限定特定数组组合,
    if(r==12)		//dx==1,dy==2
    {
        //马腿位置
        int x = step->fromRow;
        int y = (step->fromCol + step->toCol)/2;
        if(m_board[x][y] != -1)
            return false;
    }
    else
    {
        if(r==21)
        {
            int x = (step->fromRow + step->toRow)/2;
            int y = step->fromCol;
            if(m_board[x][y] != -1)
                return false;
        }
        else
            return false;
    }
    return true;
}

bool Board::checkStepPao(const Step *step)
{
    //统计两点之间有多少个棋子
    int num = countChessMan(step);
    if(step->toId != -1)	//吃子时
    {
        if(num != 1)
            return false;
    }
    else
    {
        if(num != 0)
            return false;
    }
    return true;
}

bool Board::checkRedXiang(const Step *step)
{
    if(step->toRow < 5)
        return false;		//象不过河
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;	//限定特定数字组合,
    if(r != 22)
        return false;
    int x = (step->fromRow + step->toRow)/2;
    int y = (step->fromCol + step->toCol)/2;
    if(m_board[x][y] != -1)
        return false;
    return true;
}

bool Board::checkRedShi(const Step *step)
{
    if(step->toCol <3 || step->toCol >5)
        return false;
    if(step->toRow <7 || step->toRow > 9)
        return false;
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;	//限定特定数字组合,
    if(r != 11)
        return false;
    return true;

}

bool Board::checkRedJiang(const Step *step)
{
    //排除将帅见面时终点不在本九宫格内的情况
    if(step->toId != 4)
    {
        //判断是否在九宫格内
        if(step->toCol <3 || step->toCol >5)
            return false;
        if(step->toRow <7 || step->toRow > 9)
            return false;
    }
        //判断是否将帅见面
    if(blackJiang ==m_chessMan[step->toId].type)
    {
        if(step->fromCol == step->toCol)
        {
            if(countChessMan(step) == 0)
                return true;
            else
                return false;
        }
    }
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;	//限定特定数字组合,
    if(r != 1 && r!=10)
        return false;
    return true;

}

bool Board::checkRedZu(const Step *step)
{
    if(step->toRow > step->fromRow)
        return false;		//卒子不后退
    if(step->fromRow > 4)	//未过河
    {
        if(qAbs(step->fromCol - step->toCol) == 1)
            return false;	//未过河的卒子不能左右走
    }
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;		//限定特定数字组合
    if(r==1 || r==10)
        return true;
    else
        return false;
}

bool Board::checkBlackXiang(const Step *step)
{
    if(step->toRow > 4)
        return false;		//象不过河
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;	//限定特定数组组合,
    if(r != 22)
        return false;
    int x = (step->fromRow + step->toRow)/2;
    int y = (step->fromCol + step->toCol)/2;
    if(m_board[x][y] != -1)
        return false;
    return true;
}

bool Board::checkBlackShi(const Step *step)
{
    if(step->toCol <3 || step->toCol >5)
        return false;
    if(step->toRow >2 || step->toRow < 0)
        return false;
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;	//限定特定数字组合,
    if(r != 11)
        return false;
    return true;

}

bool Board::checkBlackJiang(const Step *step)
{
    if(step->toId != 20)
    {
        //判断是否在九宫格内
        if(step->toCol <3 || step->toCol >5)
            return false;
        if(step->toRow >2 || step->toRow < 0)
            return false;
    }
        //判断是否将帅见面
    if(redJiang ==m_chessMan[step->toId].type)
    {
        if(step->fromCol == step->toCol)
        {
            if(countChessMan(step) == 0)
                return true;
            else
                return false;
        }
    }
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;	//限定特定数字组合,
    if(r != 1 && r!=10)
        return false;
    return true;

}

bool Board::checkBlackZu(const Step *step)
{
    if(step->toRow < step->fromRow)
        return false;		//卒子不后退
    if(step->fromRow < 5)	//未过河
    {
        if(qAbs(step->fromCol - step->toCol) == 1)
            return false;	//未过河的卒子不能左右走
    }
    int dx = qAbs(step->fromRow - step->toRow);
    int dy = qAbs(step->fromCol - step->toCol);
    int r = 10*dx + dy;		//限定特定数字组合
    if(r==1 || r==10)
        return true;
    else
        return false;

}

void Board::saveGame(const QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadWrite);
    QDataStream out(&file);
    //保存棋子信息
    int i;
//    i1 = chessMan[i].camp;
//    i2 = chessMan[i].state;
//    i3 = chessMan[i].type;
//    out<<i1<<i2<<i3<<chessMan[i].row<<chessMan[i].col;
    for(i=0 ; i<32 ; ++i)
    {
        out<<m_chessMan[i].camp<<m_chessMan[i].state
          <<m_chessMan[i].type<<m_chessMan[i].row<<m_chessMan[i].col;
    }
    //保存棋盘信息
    int j;
    for(i=0 ; i< 10 ; ++i)
    {
        for(j=0 ; j<9 ; ++j)
        {
            out<<m_board[i][j];
        }
    }
    //保存棋盘布局（用户的阵营）信息
    out<<m_playerCamp;
    //保存谁先行信息
    out<<m_alternate;

    file.close();
    QMessageBox::information(this,"保存","保存成功",
                             QMessageBox::Ok,QMessageBox::Ok);
}

void Board::openGame(const QString fileName)
{
    //把悔棋信息数组清空
    for(auto item : m_vStep)
    {
        delete item;
    }
    m_vStep.clear();

    //设置为没有最后走的棋子
    m_finallyMoveChessMan = -1;

    QFile file(fileName);
    file.open(QIODevice::ReadWrite);
    QDataStream in(&file);
    //读取棋子信息
    int i;
    int i1,i2,i3;
    for(i=0 ; i<32 ; ++i)
    {
        in>>i1>>i2>>i3>>m_chessMan[i].row>>m_chessMan[i].col;
        m_chessMan[i].camp = (Camp)i1;
        m_chessMan[i].state = (State)i2;
        m_chessMan[i].type = (Type)i3;
    }
    //读取棋盘信息
    int j;
    for(i=0 ; i< 10 ; ++i)
    {
        for(j=0 ; j<9 ; ++j)
        {
            in>>m_board[i][j];
        }
    }
    //读取棋盘布局信息
    in>>i1;
    m_playerCamp = (Camp)i1;
    //读取谁先行信息
    in>>i1;
    m_alternate = (Camp)i1;
    m_gameOver = false;
    file.close();
    update();
}

void Board::slotStart()
{
    m_gameOver = false;
    if(m_alternate == black)
    {
        emit changeState("当前状态 : 黑方下");
    }
    else
    {
        emit changeState("当前状态 : 红方下");
    }
    QMessageBox::information(this,"中国象棋","游戏开始",
                             QMessageBox::Ok,QMessageBox::Ok);
}


void Board::restart()
{
    //重新设置棋子和棋盘位置
    initChess();
    //重新设置游戏结束标志
    m_gameOver = false;
    //销毁vStep里所有的走步
    for(Step* item : m_vStep)
        delete item;
    //清空悔棋信息数组
    m_vStep.clear();
    //设置谁先走
    m_alternate  = m_xianCamp;
    //设置为没有最后移动的棋子
    m_finallyMoveChessMan = -1;
    update();
}



//初始化棋子和棋盘
void Board::initChess()
{
    int i,j;
    //初始化棋子

    //黑方的棋子
    m_chessMan[0] = {0,0,blackJu,black,alive};
    m_chessMan[1] = {0,1,blackMa,black,alive};
    m_chessMan[2] = {0,2,blackXiang,black,alive};
    m_chessMan[3] = {0,3,blackShi,black,alive};
    m_chessMan[4] = {0,4,blackJiang,black,alive};
    m_chessMan[5] = {0,5,blackShi,black,alive};
    m_chessMan[6] = {0,6,blackXiang,black,alive};
    m_chessMan[7] = {0,7,blackMa,black,alive};
    m_chessMan[8] = {0,8,blackJu,black,alive};
    m_chessMan[9] = {2,1,blackPao,black,alive};
    m_chessMan[10] = {2,7,blackPao,black,alive};
    m_chessMan[11] = {3,0,blackZu,black,alive};
    m_chessMan[12] = {3,2,blackZu,black,alive};
    m_chessMan[13] = {3,4,blackZu,black,alive};
    m_chessMan[14] = {3,6,blackZu,black,alive};
    m_chessMan[15] = {3,8,blackZu,black,alive};
    //红方的棋子(在数组中的位置(9-row,8-col)
    m_chessMan[16] = {9,8,redJu,red,alive};
    m_chessMan[17] = {9,7,redMa,red,alive};
    m_chessMan[18] = {9,6,redXiang,red,alive};
    m_chessMan[19] = {9,5,redShi,red,alive};
    m_chessMan[20] = {9,4,redJiang,red,alive};
    m_chessMan[21] = {9,3,redShi,red,alive};
    m_chessMan[22] = {9,2,redXiang,red,alive};
    m_chessMan[23] = {9,1,redMa,red,alive};
    m_chessMan[24] = {9,0,redJu,red,alive};
    m_chessMan[25] = {7,7,redPao,red,alive};
    m_chessMan[26] = {7,1,redPao,red,alive};
    m_chessMan[27] = {6,8,redZu,red,alive};
    m_chessMan[28] = {6,6,redZu,red,alive};
    m_chessMan[29] = {6,4,redZu,red,alive};
    m_chessMan[30] = {6,2,redZu,red,alive};
    m_chessMan[31] = {6,0,redZu,red,alive};

    //初始化棋盘状态
    //首先初始化为空
    for(i=0 ; i<9 ; ++i)
    {
        for(j=0 ; j<10 ; ++j)
        {
            m_board[i][j] = -1;
        }
    }
    //放棋子
    for(i = 0 ; i<32 ; ++i)
    {
        m_board[m_chessMan[i].row][m_chessMan[i].col] = i;
    }
}
//初始化棋子图片
void Board::initImage( Camp camp,int kind)
{
    if(kind == 1)       //加载第一种棋子
    {
        m_images[0].load(":/chessman/images/chessman1/redJu.png");
        m_images[1].load(":/chessman/images/chessman1/redMa.png");
        m_images[2].load(":/chessman/images/chessman1/redXiang.png");
        m_images[3].load(":/chessman/images/chessman1/redShi.png");
        m_images[4].load(":/chessman/images/chessman1/redJiang.png");
        m_images[5].load(":/chessman/images/chessman1/redPao.png");
        m_images[6].load(":/chessman/images/chessman1/redZu.png");

        m_images[7].load(":/chessman/images/chessman1/blackJu.png");
        m_images[8].load(":/chessman/images/chessman1/blackMa.png");
        m_images[9].load(":/chessman/images/chessman1/blackXiang.png");
        m_images[10].load(":/chessman/images/chessman1/blackShi.png");
        m_images[11].load(":/chessman/images/chessman1/blackJiang.png");
        m_images[12].load(":/chessman/images/chessman1/blackPao.png");
        m_images[13].load(":/chessman/images/chessman1/blackZu.png");

        m_scaleSize = QSize(100,100);
        m_dr = 50;
    }
    else
    {
        if(kind == 2)   //加载第二种棋子
        {
            m_images[0].load(":/chessman/images/chessman2/redJu.png");
            m_images[1].load(":/chessman/images/chessman2/redMa.png");
            m_images[2].load(":/chessman/images/chessman2/redXiang.png");
            m_images[3].load(":/chessman/images/chessman2/redShi.png");
            m_images[4].load(":/chessman/images/chessman2/redShuai.png");
            m_images[5].load(":/chessman/images/chessman2/redPao.png");
            m_images[6].load(":/chessman/images/chessman2/redZu.png");

            m_images[7].load(":/chessman/images/chessman2/blackJu.png");
            m_images[8].load(":/chessman/images/chessman2/blackMa.png");
            m_images[9].load(":/chessman/images/chessman2/blackXiang.png");
            m_images[10].load(":/chessman/images/chessman2/blackShi.png");
            m_images[11].load(":/chessman/images/chessman2/blackJiang.png");
            m_images[12].load(":/chessman/images/chessman2/blackPao.png");
            m_images[13].load(":/chessman/images/chessman2/blackZu.png");
            //记录对应的缩放尺寸
            m_scaleSize = QSize(90,90);
            //记录棋子偏移量
            m_dr = 55;
        }
        else            //加载第三种棋子
        {
            m_images[0].load(":/chessman/images/chessman/redJu.png");
            m_images[1].load(":/chessman/images/chessman/redMa.png");
            m_images[2].load(":/chessman/images/chessman/redXiang.png");
            m_images[3].load(":/chessman/images/chessman/redShi.png");
            m_images[4].load(":/chessman/images/chessman/redShuai.png");
            m_images[5].load(":/chessman/images/chessman/redPao.png");
            m_images[6].load(":/chessman/images/chessman/redBing.png");

            m_images[7].load(":/chessman/images/chessman/blackJu.png");
            m_images[8].load(":/chessman/images/chessman/blackMa.png");
            m_images[9].load(":/chessman/images/chessman/blackXiang.png");
            m_images[10].load(":/chessman/images/chessman/blackShi.png");
            m_images[11].load(":/chessman/images/chessman/blackJiang.png");
            m_images[12].load(":/chessman/images/chessman/blackPao.png");
            m_images[13].load(":/chessman/images/chessman/blackZu.png");

            m_scaleSize = QSize(90,90);
            m_dr = 55;
        }
    }
    m_imageSelectedBorder.load(":/board/redBorder");   //选择棋子时显示的红色边框
    m_imageFinallyMove.load(":/board/blueBorder");       //移动完棋子时显示的蓝色边框,提示这是最后移动的棋子
    //黑方在棋盘下方时翻转棋子
    if(camp == black)
    {
        //设置变化模式为旋转180度
        QMatrix matrix;
        matrix.rotate(180);
        for(int i=0 ; i<15 ; ++i)
        {
            //逐个倒转棋子
            m_images[i] = m_images[i].transformed(matrix);
        }
    }
}
