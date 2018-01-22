#include"mainwindow.h"
#include"netclient.h"
#include<QTextBrowser>
#include<QDebug>
#include<QDialog>
MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{

    m_selectMode = new selectDialog;
    m_selectMode->resize(370,270);
    initMenu();
    initToolBar();
    //构建控制面板对象
    m_panel = new ControlPanel(this);
    //构建锚接部件对象
    m_dockWidget = new QDockWidget("控制面板",this);


    setWindowTitle("中国象棋");


    initDockWidget();
    initStetusBar();
    //board = new NetClient(black,black,this);
    //board = new AiMode(red,black,this);
    m_board = new Board(red,red, this);


    connect(m_panel->btn1,SIGNAL(clicked(bool)),m_actSelectMode,SIGNAL(triggered(bool)));
    connect(m_panel->btn2,SIGNAL(clicked(bool)),m_actStart,SIGNAL(triggered(bool)));
    connect(m_panel->btn3,SIGNAL(clicked(bool)),m_actRestart,SIGNAL(triggered(bool)));
    connect(m_panel->btn4,SIGNAL(clicked(bool)),m_actUndoStep,SIGNAL(triggered(bool)));
    connect(m_panel->btn5,SIGNAL(clicked(bool)),m_actSave,SIGNAL(triggered(bool)));
    connect(m_panel->btn6,SIGNAL(clicked(bool)),m_actOpen,SIGNAL(triggered(bool)));
    connect(m_panel->btn7,SIGNAL(clicked(bool)),m_actQuit,SIGNAL(triggered(bool)));

    connect(m_actCtrlPanel,SIGNAL(triggered(bool)),this,SLOT(slot_ctrlPanel()));
    connect(m_actGameRule,SIGNAL(triggered(bool)),this,SLOT(slot_showHelp()));
    connect(m_actSelectMode,SIGNAL(triggered(bool)),this,SLOT(slot_select()));
    connect(m_actQuit,SIGNAL(triggered(bool)),this,SLOT(close()));



//选择模式后需要重新设置的信息
    //设置board为中心部件
    m_board->setMinimumSize(450,550);
    setCentralWidget(m_board);


    //通过信号和槽通信来改变状态栏的显示信息
    connect(m_actSave,SIGNAL(triggered(bool)),m_board,SLOT(slotSave()));
    connect(m_actOpen,SIGNAL(triggered(bool)),m_board,SLOT(slotOpen()));
    connect(m_board,SIGNAL(changeState(const QString)),this,SLOT(slot_setState(const QString)));
    connect(m_board,SIGNAL(changeMode(const QString)),this,SLOT(slot_setMode(const QString)));
    connect(m_actStart,SIGNAL(triggered(bool)),m_board,SLOT(slotStart()));
    connect(m_actRestart,SIGNAL(triggered(bool)),m_board,SLOT(restart()));
    connect(m_actUndoStep,SIGNAL(triggered(bool)),m_board,SLOT(undoStep()));



}

void MainWindow::initMenu()
{

    //创建菜单
    m_menuFile = new QMenu("文件(&F)",this);
    m_menuVier = new QMenu("视图(&V)",this);
    m_menuOperator = new QMenu("操作(&O)",this);
    m_menuHelp = new QMenu("帮助(&H)",this);
    //加入菜单栏
    menuBar()->addMenu(m_menuFile);
    menuBar()->addMenu(m_menuVier);
    menuBar()->addMenu(m_menuOperator);
    menuBar()->addMenu(m_menuHelp);
    //文件菜单的动作
    m_actSave = new QAction(QIcon(":icon/images/icon/save.png"),"保存",this);
    m_actOpen = new QAction(QIcon(":icon/images/icon/open.png"),"载入",this);
    m_menuFile->addAction(m_actSave);
    m_menuFile->addAction(m_actOpen);
    //视图菜单的动作
    m_actCtrlPanel = new QAction(QIcon(":icon/images/icon/splash.png"),"控制面板",this);
    m_menuVier->addAction(m_actCtrlPanel);
    //操作菜单的动作
    m_actQuit = new QAction(QIcon(":icon/images/icon/craven.png"),"退出",this);
    m_actSelectMode = new QAction(QIcon(":icon/images/icon/substitution.png"),"选择模式",this);
    m_actStart = new QAction(QIcon(":icon/images/icon/pause.png"),"开始",this);
    m_actRestart = new QAction(QIcon(":icon/images/icon/begin2.png"),"重新开始",this);
    m_actUndoStep = new QAction(QIcon(":icon/images/icon/undo.png"),"悔棋",this);
    //actSets = new QAction("设置");
    //actMagnify = new QAction("棋盘放大");
    //actShrink = new QAction("棋盘缩小");
    //actAdapt = new QAction("棋盘适应窗口");
    m_menuOperator->addAction(m_actSelectMode);
    m_menuOperator->addAction(m_actStart);
    m_menuOperator->addAction(m_actRestart);
    m_menuOperator->addAction(m_actUndoStep);
    m_menuOperator->addAction(m_actQuit);
    //帮助菜单动作
    m_actGameRule = new QAction(QIcon(":/chessman/images/chessman2/redShuai.png"),"游戏规则",this);
    m_menuHelp->addAction(m_actGameRule);


}

void MainWindow::initDockWidget()
{
    //设置锚接部件在主窗口中可以停靠的位置,设置为在任意一个方位都可停靠
    m_dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_dockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    //设置锚接部件初始显示时不浮动,即第一次显示时会停靠在主窗口中
    m_dockWidget->setFloating(false);
    //将控制面板部件放到停靠部件中
    m_dockWidget->setWidget(m_panel);
    //设置锚接部件为可见
    m_dockWidget->setVisible(true);
    //将锚接部件加入到主窗口中
    addDockWidget(Qt::RightDockWidgetArea,m_dockWidget);
}

void MainWindow::initToolBar()
{
    QToolBar *bar = new QToolBar(this);
    //设置工具栏可放置的位置
    bar->setAllowedAreas(Qt::AllToolBarAreas);
    //设置工具栏图标的排列方向
    bar->setOrientation(Qt::Horizontal);
    //添加动作
    bar->addAction(m_actSave);
    bar->addAction(m_actOpen);
    bar->addAction(m_actStart);
    bar->addAction(m_actRestart);
    bar->addAction(m_actUndoStep);
    bar->addAction(m_actCtrlPanel);
    bar->addAction(m_actSelectMode);
    bar->addAction(m_actQuit);
    //将工具栏添加到主窗口,
    addToolBar(Qt::TopToolBarArea,bar);
}

void MainWindow::initStetusBar()
{
#if 1
    //statusBar()获取应用程序主窗口的状态栏
    QStatusBar *bar = statusBar();
    m_labelMode = new QLabel("默认模式 : 人人对战 , 执红 , 红方先行  ");
    //设置标签最小尺寸,在这种情况下,标签不会伸缩,除非无法显示全部信息
  //  label1->setMinimumSize(200,25);
    //设置标签的形状
    m_labelMode->setFrameShape(QFrame::WinPanel);
    //设置标签外框的阴影
    m_labelMode->setFrameShadow(QFrame::Sunken);

    m_labelXia = new QLabel;//("当前状态 : 红方下  ");

    m_labelXia->setMinimumSize(200,25);
    m_labelXia->setFrameShape(QFrame::Panel);
    m_labelXia->setFrameShadow(QFrame::Sunken);

    bar->addWidget(m_labelMode);
    bar->addWidget(m_labelXia);

    QLabel *per1 = new QLabel("就绪", this);
    statusBar()->addPermanentWidget(per1); //实现永久信息
//    per1->setMinimumSize(60,25);
//    per1->setFrameShape(QFrame::Panel);
//    per1->setFrameShadow(QFrame::Sunken);

#endif

}

void MainWindow::setBoard()
{
    m_board->setMinimumSize(450,550);
    setCentralWidget(m_board);

    //重新连接信号和槽
    //通过信号和槽通信来改变状态栏的显示信息
    connect(m_actSave,SIGNAL(triggered(bool)),m_board,SLOT(slotSave()));
    connect(m_actOpen,SIGNAL(triggered(bool)),m_board,SLOT(slotOpen()));
    connect(m_board,SIGNAL(changeState(const QString)),this,SLOT(slot_setState(const QString)));
    connect(m_board,SIGNAL(changeMode(const QString)),this,SLOT(slot_setMode(const QString)));
    connect(m_actStart,SIGNAL(triggered(bool)),m_board,SLOT(slotStart()));
    connect(m_actRestart,SIGNAL(triggered(bool)),m_board,SLOT(restart()));
    connect(m_actUndoStep,SIGNAL(triggered(bool)),m_board,SLOT(undoStep()));


}

//void MainWindow::slotAiMode()
//{

//    //设置board为中心部件
//    board = new AiMode(red,red,this);
//    board->setMinimumSize(450,550);
//    setCentralWidget(board);

//    //连接信号和槽
//    connect(panel->btn1,SIGNAL(clicked(bool)),board,SLOT(restart()));
//    connect(panel->btn2,SIGNAL(clicked(bool)),board,SLOT(undoStep()));
//    connect(panel->btn4,SIGNAL(clicked(bool)),board,SLOT(slotStart()));
//    connect(panel->btn5,SIGNAL(clicked(bool)),board,SLOT(slotRedXian()));
//    connect(panel->btn6,SIGNAL(clicked(bool)),board,SLOT(slotDeconnect()));
//    connect(panel->btn7,SIGNAL(clicked(bool)),board,SLOT(slotBlackXian()));

//}


//Board(Camp xian, Camp playerCamp=red, QWidget *parent = 0);
//NetClient(Camp camp,Camp playerCamp,QWidget *parent=0);
//AiMode(Camp xian,Camp playerCamp,QWidget *parent=0,int level = 5,);

//选择模式
void MainWindow::slot_select()
{
    //
    if(m_selectMode->exec()==QDialog::Accepted)
    {
        if((m_selectMode->m_stack->m_leftlist->currentRow()) == 0)
        {
            Camp xian,player;
            m_selectMode->m_stack->m_multiPage->setting(xian,player);
            m_board = new Board(xian,player);
            setBoard();         //重新连接信号和槽

            //"默认模式 : 人人对战 , 执红 , 红方先行  "
            QString str = "当前模式 : 人人对战 , ";
            if(player == red)
                str += "执红 , ";
            else
                str += "执黑 , ";
            if(xian == red)
                str += "红方先行  ";
            else
                str +="黑方先行";
            m_labelMode->setText(str);
        }
        else         //人机对战
        {
            if(m_selectMode->m_stack->m_leftlist->currentRow() == 1)
            {
                Camp xian,player;
                int difficulty;
                m_selectMode->m_stack->m_aiPage->setting(xian,player,difficulty);
                m_board = new AiMode(xian,player,this,difficulty);
                //不显示保存和载入按钮(不能用这两个功能);
                m_panel->btn5->setVisible(false);
                m_panel->btn6->setVisible(false);
                //禁用这两个QAction
                m_actSave->setEnabled(false);
                m_actOpen->setEnabled(false);

                setBoard();		//重新连接信号和槽

                //"默认模式 : 人人对战 , 执红 , 红方先行  "
                QString str = "当前模式 : 人机对战 , ";
                if(player == red)
                    str += "执红 , ";
                else
                    str += "执黑 , ";
                if(xian == red)
                    str += "红方先行  ";
                else
                    str +="黑方先行";
                m_labelMode->setText(str);
            }
            else      //网络对战
            {
                if(m_selectMode->m_stack->m_leftlist->currentRow() == 2)
                {
                    Camp xian,player;
                    m_selectMode->m_stack->m_netPage->setting(xian,player);
                    m_board = new NetClient(xian,player);

                    //不显示保存和载入按钮(不能用这两个功能);
                    m_panel->btn5->setVisible(false);
                    m_panel->btn6->setVisible(false);
                    //禁用这两个QAction
                    m_actSave->setEnabled(false);
                    m_actOpen->setEnabled(false);


                    setBoard();


                    //"默认模式 : 人人对战 , 执红 , 红方先行  "
                    QString str = "当前模式 : 网络对战 , ";
                    if(player == red)
                        str += "执红 , ";
                    else
                        str += "执黑 , ";
                    if(xian == red)
                        str += "红方先行  ";
                    else
                        str +="黑方先行";
                    m_labelMode->setText(str);


                    static_cast<NetClient*>(m_board)->slot_tryConnect(player,xian);

                }
            }
        }
        update();
    }
}

void MainWindow::slot_setState(const QString str)
{
    m_labelXia->setText(str);
}

void MainWindow::slot_setMode(const QString str)
{
    m_labelMode->setText(str);
}

void MainWindow::slot_ctrlPanel()
{
    //设置锚接部件为可见
    m_dockWidget->setVisible(true);
}

void MainWindow::slot_showHelp()
{
    Rule *r = new Rule;
    //设置关闭时销毁
    r->setAttribute(Qt::WA_DeleteOnClose);
    r->show();

}
