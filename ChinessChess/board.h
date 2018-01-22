#ifndef BOARD_H
#define BOARD_H
#include<QDebug>
#include<QPainter>
#include<QMessageBox>
#include<QMouseEvent>
#include<QWidget>
#include<QPoint>
#include<QPointF>
#include<QTimer>
#include<QImage>
#include<vector>
using std::vector;
#include"chessman.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    //构造函数,
    Board(Camp xian, Camp playerCamp=red, QWidget *parent = 0);
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    //重写resizeEvent,当大小改变时,重新设置格子宽度和高度
    void resizeEvent(QResizeEvent *event);
protected:
    Camp m_playerCamp = red;      //记录玩家的阵营,初始化为red
    Camp m_xianCamp;
    QPoint m_windowSize;          //记录窗口尺寸
    //记录该哪一方落子了, 初始化为red,同时起到控制哪一方先走
    Camp m_alternate = red;
    ChessMan m_chessMan[32];      //记录32颗棋子的状态
    signed char m_board[10][9];   //存储棋盘每个点的状态
    QImage m_images[14];          //14种棋子图片
    QImage m_imageSelectedBorder; //选择棋子时显示的红色边框
    QImage m_imageFinallyMove;    //移动完棋子时显示的蓝色边框,提示这是最后移动的棋子
    QSize m_scaleSize;            //缩放尺寸
    int m_dr;                     //画棋子时确定偏移量(其值为100-半径);

    //用于获取鼠标点击对应的数组中的行和列值
    int m_boardGridWidth;         //记录当前棋盘格子的宽度
    int m_boardGridHeigh;         //记录当前棋盘格子的高度
    char m_selectedChessMan = -1;   //记录当前选择的棋子,为-1时表示没有选择棋子
    char m_finallyMoveChessMan = -1;  //记录最后移动的棋子,-1表示没有最后移动的棋子
    vector<Step*> m_vStep;			//记录走棋信息,便于悔棋
    //先初始化为true,刚开始时不让走,点击开始按钮才让走
    bool m_gameOver = true;		//记录游戏是否结束,为true时表示结束
    Step *m_step;
    QPixmap m_pixmapBacks;        //加载棋盘图片

protected:
    //初始化棋子和棋盘
    void initChess();
    //初始化棋子图片
    void initImage(Camp camp = red, int kind=1);
    //把屏幕坐标转换为对应的数组的行列值
    void coordinateToRow(QPoint point, int &row, int &col);    
    //判断行列值是否合法,合法返回true,不合法返回false;
    bool judgeRow(const int &row , const int &col);
    //移动棋子(从(fromRow,fromCol)移动到(toRow,toCol));假设位置都合法,走的这一步合法
    void moveChessMan(const Step &step);
    //把棋子移回去(重载函数)
    void moveChessMan(const Step &step,int);
    //记录走棋信息
    void recordStep(Step *step);
    //黑方在棋子下方时把点击对应的(row,col)转换为程序内部使用的数组的(row,col)
    void toClientRow(int &row,int &col);
    //生成走步,假设selectedChessMan,row,col都合法
    Step *generateStep(int selectedChessMan,int row,int col);
    //设置成下一方走棋
    void alternateNextPlayer();
    //处理点击的点,如果确实走棋了则返回true,
    bool playerClick(int row,int col);
    //判断游戏是否结束,返回true表示结束,winner里保存胜者的阵营
    bool whetherTheGameIsOver(Camp &winner);
    //判断棋子是否能走,如果能走则返回true,否则返回false
    bool checkStep(const Step *step);

    //计算两点之间的棋子个数并返回,-1表示两点不在一条线上
    int countChessMan(const Step *step);
    //画棋子
    void paintChessMan(QPainter &painter, QPixmap &pixmap);
protected:
    //step里的点都在棋盘内,检测各棋子的走步是否合法,黑方棋子在棋盘数组的上方
    //合法时返回true;
    bool checkStepJu(const Step *step);
    bool checkStepMa(const Step *step);
    bool checkStepPao(const Step *step);
    bool checkRedXiang(const Step *step);
    bool checkRedShi(const Step *step);
    bool checkRedJiang(const Step *step);
    bool checkRedZu(const Step *step);
    bool checkBlackXiang(const Step *step);
    bool checkBlackShi(const Step *step);
    bool checkBlackJiang(const Step *step);
    bool checkBlackZu(const Step *step);
    //保存游戏,只在人人对战时有用
    void saveGame(const QString fileName);
    void openGame(const QString fileName);
    QString file = "chess";
signals:
    void signal_changeState(const QString str);
    void signal_changeMode(const QString str);
public slots:
    void slot_save()
    {
        saveGame(file);
    }

    void slot_open()
    {
        openGame(file);
    }

    //开始
    virtual void slot_start();
    //重新开始
    virtual void slot_restart();
    //悔棋
    virtual void slot_undoStep();
    void slot_showRedVictory();
    void slot_showBlackVictory();
    void slot_redXian();
    void slot_blackXian();
    virtual void slot_initialize();      //初始化时

};


#endif // BOARD_H
