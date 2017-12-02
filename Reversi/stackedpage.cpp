#include "stackedpage.h"

MultiPage::MultiPage(QWidget *parent)
    :QWidget(parent)
{
#if 0
    lAhead = new QLabel("规则:\n"
                        "\t黑白棋，又叫翻转棋（Reversi）、奥赛罗棋（Othello）、\n"
                        "苹果棋或反棋（Anti reversi）。\n"
                        "\t黑白棋在西方和日本很流行。\n"
                        "\t游戏通过相互翻转对方的棋子，最后以棋盘上谁的棋子多来判断胜负。\n"
                        "\t它的游戏规则简单，因此上手很容易，但是它的变化又非常复杂。\n"
                        "\t有一种说法是：只需要几分钟学会它，却需要一生的时间去精通它。");
#endif
    lAhead = new QLabel("当前选择: 人人对战");
    lAhead->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->addWidget(lAhead);
}


AiPage::AiPage(QWidget *parent)
    :QWidget(parent)
{
    lAhead = new QLabel("请选择:");
        lAhead->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    comAhead = new QComboBox;
        comAhead->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    comAhead->addItem("执黑(先行)");
    comAhead->addItem("执白");
    lDifficulty = new QLabel("难度");
        lDifficulty->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    comDifficulty = new QComboBox;
        comDifficulty->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    comDifficulty->addItem("初级");
    comDifficulty->addItem("中级");
    comDifficulty->addItem("高级");

    //设置不可编辑
    comAhead->setEditable(false);
    comDifficulty->setEditable(false);

    QGridLayout *lay = new QGridLayout(this);
    lay->addWidget(lAhead,0,0);
    lay->addWidget(comAhead,0,1);
    lay->addWidget(lDifficulty,1,0);
    lay->addWidget(comDifficulty,1,1);
}
//ahead为false时是执黑(先走),difficulty是难度
void AiPage::setting(bool  &ahead, int &difficulty)
{
    if(comAhead->currentIndex() == 0)
    {
        //执黑
        ahead = false;
    }
    else
        ahead = true;      //执白
    if(comDifficulty->currentIndex() == 0)
        difficulty = 2;
    else
    {
        if(comDifficulty->currentIndex() == 1)
            difficulty = 4;
        else
            difficulty = 6;
    }
}


NetPage::NetPage(QWidget *parent)
    :QWidget(parent)
{
    lMode = new QLabel("请选择:");
        lMode->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    comMode = new QComboBox;
        comMode->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    comMode->addItem("服务器(执黑先行)");
    comMode->addItem("客户端(执白)");
    comMode->addItem("客户端(执黑先行)");
    comMode->addItem("服务器(执白");

    //设置为不可编辑
    comMode->setEditable(false);

    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->addWidget(lMode);
    lay->addWidget(comMode);
}

void NetPage::setting(bool &isServer, bool &color)
{
    if(comMode->currentIndex() == 0 )
    {
        //选的是服务器(执黑先行)
        isServer = true;
        color = false;
    }
    else
    {
        if(comMode->currentIndex() == 1)
        {
            //客户端(执白)
            isServer = false;
            color = true;
        }
        else
        {
            if(comMode->currentIndex() == 2)
            {   //客户端(执黑先行)
                isServer = false;
                color = false;
            }
            else
            {   //服务器端(执白)
                isServer = true;
                color = true;
            }

        }

    }
}

