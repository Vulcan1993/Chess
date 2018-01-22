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
    m_lAhead = new QLabel("当前选择: 人人对战");
    m_lAhead->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->addWidget(m_lAhead);
}


AiPage::AiPage(QWidget *parent)
    :QWidget(parent)
{
    m_lAhead = new QLabel("请选择:");
        m_lAhead->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    m_comAhead = new QComboBox;
        m_comAhead->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    m_comAhead->addItem("执黑(先行)");
    m_comAhead->addItem("执白");
    m_lDifficulty = new QLabel("难度");
        m_lDifficulty->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    m_comDifficulty = new QComboBox;
        m_comDifficulty->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    m_comDifficulty->addItem("初级");
    m_comDifficulty->addItem("中级");
    m_comDifficulty->addItem("高级");

    //设置不可编辑
    m_comAhead->setEditable(false);
    m_comDifficulty->setEditable(false);

    QGridLayout *lay = new QGridLayout(this);
    lay->addWidget(m_lAhead,0,0);
    lay->addWidget(m_comAhead,0,1);
    lay->addWidget(m_lDifficulty,1,0);
    lay->addWidget(m_comDifficulty,1,1);
}
//ahead为false时是执黑(先走),difficulty是难度
void AiPage::setting(bool  &ahead, int &difficulty)
{
    if(m_comAhead->currentIndex() == 0)
    {
        //执黑
        ahead = false;
    }
    else
        ahead = true;      //执白
    if(m_comDifficulty->currentIndex() == 0)
        difficulty = 2;
    else
    {
        if(m_comDifficulty->currentIndex() == 1)
            difficulty = 4;
        else
            difficulty = 6;
    }
}


NetPage::NetPage(QWidget *parent)
    :QWidget(parent)
{
    m_lMode = new QLabel("请选择:");
        m_lMode->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    m_comMode = new QComboBox;
        m_comMode->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    m_comMode->addItem("服务器(执黑先行)");
    m_comMode->addItem("客户端(执白)");
    m_comMode->addItem("客户端(执黑先行)");
    m_comMode->addItem("服务器(执白");

    //设置为不可编辑
    m_comMode->setEditable(false);

    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->addWidget(m_lMode);
    lay->addWidget(m_comMode);
}

void NetPage::setting(bool &isServer, bool &color)
{
    if(m_comMode->currentIndex() == 0 )
    {
        //选的是服务器(执黑先行)
        isServer = true;
        color = false;
    }
    else
    {
        if(m_comMode->currentIndex() == 1)
        {
            //客户端(执白)
            isServer = false;
            color = true;
        }
        else
        {
            if(m_comMode->currentIndex() == 2)
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

