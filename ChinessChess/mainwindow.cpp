#include"mainwindow.h"
#include"netclient.h"
#include<QTextBrowser>
#include<QDebug>
#include<QDialog>
MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
{

    selectMode = new selectDialog;
    selectMode->resize(370,270);
    initMenu();
    initToolBar();
    //构建控制面板对象
    panel = new ControlPanel(this);
    //构建锚接部件对象
    dockWidget = new QDockWidget("控制面板",this);


    setWindowTitle("中国象棋");


    initDockWidget();
    initStetusBar();
    //board = new NetClient(black,black,this);
    //board = new AiMode(red,black,this);
    board = new Board(red,red, this);


    connect(panel->btn1,SIGNAL(clicked(bool)),actSelectMode,SIGNAL(triggered(bool)));
    connect(panel->btn2,SIGNAL(clicked(bool)),actStart,SIGNAL(triggered(bool)));
    connect(panel->btn3,SIGNAL(clicked(bool)),actRestart,SIGNAL(triggered(bool)));
    connect(panel->btn4,SIGNAL(clicked(bool)),actUndoStep,SIGNAL(triggered(bool)));
    connect(panel->btn5,SIGNAL(clicked(bool)),actSave,SIGNAL(triggered(bool)));
    connect(panel->btn6,SIGNAL(clicked(bool)),actOpen,SIGNAL(triggered(bool)));
    connect(panel->btn7,SIGNAL(clicked(bool)),actQuit,SIGNAL(triggered(bool)));

    connect(actCtrlPanel,SIGNAL(triggered(bool)),this,SLOT(slotCtrlPanel()));
    connect(actGameRule,SIGNAL(triggered(bool)),this,SLOT(slotShowHelp()));
    connect(actSelectMode,SIGNAL(triggered(bool)),this,SLOT(slotSelect()));
    connect(actQuit,SIGNAL(triggered(bool)),this,SLOT(close()));



//选择模式后需要重新设置的信息
    //设置board为中心部件
    board->setMinimumSize(450,550);
    setCentralWidget(board);


    //通过信号和槽通信来改变状态栏的显示信息
    connect(actSave,SIGNAL(triggered(bool)),board,SLOT(slotSave()));
    connect(actOpen,SIGNAL(triggered(bool)),board,SLOT(slotOpen()));
    connect(board,SIGNAL(changeState(const QString)),this,SLOT(slotSetState(const QString)));
    connect(board,SIGNAL(changeMode(const QString)),this,SLOT(slotSetMode(const QString)));
    connect(actStart,SIGNAL(triggered(bool)),board,SLOT(slotStart()));
    connect(actRestart,SIGNAL(triggered(bool)),board,SLOT(restart()));
    connect(actUndoStep,SIGNAL(triggered(bool)),board,SLOT(undoStep()));



}

void MainWindow::initMenu()
{

    //创建菜单
    menuFile = new QMenu("文件(&F)",this);
    menuVier = new QMenu("视图(&V)",this);
    menuOperator = new QMenu("操作(&O)",this);
    menuHelp = new QMenu("帮助(&H)",this);
    //加入菜单栏
    menuBar()->addMenu(menuFile);
    menuBar()->addMenu(menuVier);
    menuBar()->addMenu(menuOperator);
    menuBar()->addMenu(menuHelp);
    //文件菜单的动作
    actSave = new QAction(QIcon(":icon/images/icon/save.png"),"保存",this);
    actOpen = new QAction(QIcon(":icon/images/icon/open.png"),"载入",this);
    menuFile->addAction(actSave);
    menuFile->addAction(actOpen);
    //视图菜单的动作
    actCtrlPanel = new QAction(QIcon(":icon/images/icon/splash.png"),"控制面板",this);
    menuVier->addAction(actCtrlPanel);
    //操作菜单的动作
    actQuit = new QAction(QIcon(":icon/images/icon/craven.png"),"退出",this);
    actSelectMode = new QAction(QIcon(":icon/images/icon/substitution.png"),"选择模式",this);
    actStart = new QAction(QIcon(":icon/images/icon/pause.png"),"开始",this);
    actRestart = new QAction(QIcon(":icon/images/icon/begin2.png"),"重新开始",this);
    actUndoStep = new QAction(QIcon(":icon/images/icon/undo.png"),"悔棋",this);
    //actSets = new QAction("设置");
    //actMagnify = new QAction("棋盘放大");
    //actShrink = new QAction("棋盘缩小");
    //actAdapt = new QAction("棋盘适应窗口");
    menuOperator->addAction(actSelectMode);
    menuOperator->addAction(actStart);
    menuOperator->addAction(actRestart);
    menuOperator->addAction(actUndoStep);
    menuOperator->addAction(actQuit);
    //帮助菜单动作
    actGameRule = new QAction(QIcon(":/chessman/images/chessman2/redShuai.png"),"游戏规则",this);
    menuHelp->addAction(actGameRule);


}

void MainWindow::initDockWidget()
{
    //设置锚接部件在主窗口中可以停靠的位置,设置为在任意一个方位都可停靠
    dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    //设置锚接部件初始显示时不浮动,即第一次显示时会停靠在主窗口中
    dockWidget->setFloating(false);
    //将控制面板部件放到停靠部件中
    dockWidget->setWidget(panel);
    //设置锚接部件为可见
    dockWidget->setVisible(true);
    //将锚接部件加入到主窗口中
    addDockWidget(Qt::RightDockWidgetArea,dockWidget);
}

void MainWindow::initToolBar()
{
    QToolBar *bar = new QToolBar(this);
    //设置工具栏可放置的位置
    bar->setAllowedAreas(Qt::AllToolBarAreas);
    //设置工具栏图标的排列方向
    bar->setOrientation(Qt::Horizontal);
    //添加动作
    bar->addAction(actSave);
    bar->addAction(actOpen);
    bar->addAction(actStart);
    bar->addAction(actRestart);
    bar->addAction(actUndoStep);
    bar->addAction(actCtrlPanel);
    bar->addAction(actSelectMode);
    bar->addAction(actQuit);
    //将工具栏添加到主窗口,
    addToolBar(Qt::TopToolBarArea,bar);
}

void MainWindow::initStetusBar()
{
#if 1
    //statusBar()获取应用程序主窗口的状态栏
    QStatusBar *bar = statusBar();
    labelMode = new QLabel("默认模式 : 人人对战 , 执红 , 红方先行  ");
    //设置标签最小尺寸,在这种情况下,标签不会伸缩,除非无法显示全部信息
  //  label1->setMinimumSize(200,25);
    //设置标签的形状
    labelMode->setFrameShape(QFrame::WinPanel);
    //设置标签外框的阴影
    labelMode->setFrameShadow(QFrame::Sunken);

    labelXia = new QLabel;//("当前状态 : 红方下  ");

    labelXia->setMinimumSize(200,25);
    labelXia->setFrameShape(QFrame::Panel);
    labelXia->setFrameShadow(QFrame::Sunken);

    bar->addWidget(labelMode);
    bar->addWidget(labelXia);

    QLabel *per1 = new QLabel("就绪", this);
    statusBar()->addPermanentWidget(per1); //实现永久信息
//    per1->setMinimumSize(60,25);
//    per1->setFrameShape(QFrame::Panel);
//    per1->setFrameShadow(QFrame::Sunken);

#endif

}

void MainWindow::setBoard()
{
    board->setMinimumSize(450,550);
    setCentralWidget(board);

    //重新连接信号和槽
    //通过信号和槽通信来改变状态栏的显示信息
    connect(actSave,SIGNAL(triggered(bool)),board,SLOT(slotSave()));
    connect(actOpen,SIGNAL(triggered(bool)),board,SLOT(slotOpen()));
    connect(board,SIGNAL(changeState(const QString)),this,SLOT(slotSetState(const QString)));
    connect(board,SIGNAL(changeMode(const QString)),this,SLOT(slotSetMode(const QString)));
    connect(actStart,SIGNAL(triggered(bool)),board,SLOT(slotStart()));
    connect(actRestart,SIGNAL(triggered(bool)),board,SLOT(restart()));
    connect(actUndoStep,SIGNAL(triggered(bool)),board,SLOT(undoStep()));


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
void MainWindow::slotSelect()
{
    //
    if(selectMode->exec()==QDialog::Accepted)
    {
        if((selectMode->s->leftlist->currentRow()) == 0)
        {
            Camp xian,player;
            selectMode->s->m->setting(xian,player);
            board = new Board(xian,player);
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
            labelMode->setText(str);
        }
        else         //人机对战
        {
            if(selectMode->s->leftlist->currentRow() == 1)
            {
                Camp xian,player;
                int difficulty;
                selectMode->s->a->setting(xian,player,difficulty);
                board = new AiMode(xian,player,this,difficulty);
                //不显示保存和载入按钮(不能用这两个功能);
                panel->btn5->setVisible(false);
                panel->btn6->setVisible(false);
                //禁用这两个QAction
                actSave->setEnabled(false);
                actOpen->setEnabled(false);

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
                labelMode->setText(str);
            }
            else      //网络对战
            {
                if(selectMode->s->leftlist->currentRow() == 2)
                {
                    Camp xian,player;
                    selectMode->s->n->setting(xian,player);
                    board = new NetClient(xian,player);

                    //不显示保存和载入按钮(不能用这两个功能);
                    panel->btn5->setVisible(false);
                    panel->btn6->setVisible(false);
                    //禁用这两个QAction
                    actSave->setEnabled(false);
                    actOpen->setEnabled(false);


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
                    labelMode->setText(str);


                    static_cast<NetClient*>(board)->slotTryConnect(player,xian);

                }
            }
        }
        update();
    }
}

void MainWindow::slotSetState(const QString str)
{
    labelXia->setText(str);
}

void MainWindow::slotSetMode(const QString str)
{
    labelMode->setText(str);
}

void MainWindow::slotCtrlPanel()
{
    //设置锚接部件为可见
    dockWidget->setVisible(true);
}

void MainWindow::slotShowHelp()
{
    Rule *r = new Rule;
    //设置关闭时销毁
    r->setAttribute(Qt::WA_DeleteOnClose);
    r->show();

}
