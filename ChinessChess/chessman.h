#ifndef CHESSMAN_H
#define CHESSMAN_H
//#include"board.h"
#include"compconfig.h"

enum Type {redJu=0,redMa,redXiang,redShi,redJiang,redPao,redZu,
           blackJu,blackMa,blackXiang,blackShi,blackJiang,blackPao,blackZu,
            kong
          };
//表示阵营,即是红还是黑
enum Camp {red=0,black};
//表示是否出局
enum State {alive=0,died};


//棋子信息类
struct ChessMan
{
    int row;                //在数组中的行下标
    int col;                //在数组中的列下标
    Type type;		//棋子类型,选择图片时用
    Camp camp;		//记录属于哪个阵营(红方or黑方)
    State state;     //记录棋子是否出局,若为false则出局
    //id实际可用下标代替
    //unsigned char id;       //记录棋子id,因为只有32个棋子

};

//struct ChessBoard
//{
//    //Type type;                //记录在这个位置的棋子的棋子类型
//    int id;         //记录哪颗棋子在这个位置,-1表示没有棋子
//    //Camp camp;      //记录棋子属于哪个阵营
//};

struct Step
{
    //起始位置
    int fromRow;
    int fromCol;
    int fromId;
    //终止位置
    int toRow;
    int toCol;
    int toId;
};

#endif // CHESSMAN_H
