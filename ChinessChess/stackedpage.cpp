#include "stackedpage.h"
MultiPage::MultiPage(QWidget *parent)
    :QWidget(parent)
{
    m_lAhead = new QLabel("请选择: ");
    m_lAhead->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    m_lCamp = new QLabel("请选择:");
        m_lCamp->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    m_cAhead = new QComboBox;
        m_cAhead->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    m_cAhead->setEditable(false);
    m_cAhead->addItem("红方先行");
    m_cAhead->addItem("黑方先行");
    m_comCamp = new QComboBox;
        m_comCamp->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    m_comCamp->addItem("红方在棋盘下方");
    m_comCamp->addItem("黑方在棋盘下方");
    QGridLayout *lay = new QGridLayout(this);
    lay->addWidget(m_lAhead,0,0);
    lay->addWidget(m_cAhead,0,1);
    lay->addWidget(m_lCamp,1,0);
    lay->addWidget(m_comCamp,1,1);
}

void MultiPage::setting(Camp &xian, Camp &player)
{
    if(m_comCamp->currentIndex() == 0)
        player = red;
    else
        player = black;

    if(m_cAhead->currentIndex() == 0)
        xian = red;
    else
        xian = black;
}


AiPage::AiPage(QWidget *parent)
    :QWidget(parent)
{
    lCamp = new QLabel("请选择阵营:");
        lCamp->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    lAhead = new QLabel("先行或后行:");
        lAhead->setStyleSheet("QLabel{font:bold 16px; color:blue}");

    comCamp = new QComboBox;
        comCamp->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    comCamp->addItem("执红");
    comCamp->addItem("执黑");
    comCamp->setEditable(false);

    comAhead = new QComboBox;
        comAhead->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    comAhead->addItem("红方先行");
    comAhead->addItem("黑方先行");
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
    lay->addWidget(lCamp,0,0);
    lay->addWidget(comCamp,0,1);
    lay->addWidget(lAhead,1,0);
    lay->addWidget(comAhead,1,1);
    lay->addWidget(lDifficulty,2,0);
    lay->addWidget(comDifficulty,2,1);
}

void AiPage::setting(Camp &xian, Camp &player, int &difficulty)
{
    if(comCamp->currentIndex() == 0)
        player = red;
    else
        player = black;
    if(comAhead->currentIndex() == 0)
        xian = red;
    else
        xian = black;
    if(comDifficulty->currentIndex() == 0)
    {
        difficulty = 2;
        return;
    }
    if(comDifficulty->currentIndex() == 1)
        difficulty = 4;
    else
        difficulty = 6;
}
NetPage::NetPage(QWidget *parent)
    :QWidget(parent)
{
    lCamp = new QLabel("请选择阵营:");
        lCamp->setStyleSheet("QLabel{font:bold 16px; color:blue}");
    lAhead = new QLabel("先行或后行:");
        lAhead->setStyleSheet("QLabel{font:bold 16px; color:blue}");

    comCamp = new QComboBox;
        comCamp->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    comCamp->addItem("执红");
    comCamp->addItem("执黑");
    comCamp->setEditable(false);

    comAhead = new QComboBox;
        comAhead->setStyleSheet("QComboBox{font:bold 16px; color:blue}");
    comAhead->addItem("红方先行");
    comAhead->addItem("黑方先行");

    //设置为不可编辑
    comCamp->setEditable(false);
    comAhead->setEditable(false);

    QGridLayout *lay = new QGridLayout(this);
    lay->addWidget(lCamp,0,0);
    lay->addWidget(comCamp,0,1);
    lay->addWidget(lAhead,1,0);
    lay->addWidget(comAhead,1,1);
}

void NetPage::setting(Camp &xian, Camp &player)
{
    if(comCamp->currentIndex() == 0)
        player = red;
    else
        player = black;
    if(comAhead->currentIndex() == 0)
        xian = red;
    else
        xian = black;

}
