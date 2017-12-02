#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QMenu>
#include<QAction>
#include<QToolBar>
#include<QStatusBar>
#include<QMenuBar>
#include<QPushButton>
#include<QLayout>
#include<QDockWidget>
#include<QLabel>
#include<QAction>

#include"board.h"
#include"controlpanel.h"
#include"aimode.h"
#include"selectdialog.h"
#include"rule.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private:

    //初始化菜单栏
    void initMenu();
    //初始化锚接部件
    void initDockWidget();
    //初始化工具栏
    void initToolBar();
    //初始化状态栏
    void initStetusBar();

 //   void init
private:

    selectDialog *selectMode;         //选择对话框
    ControlPanel *panel;
    QDockWidget *dockWidget;
    Board *board;
    QLabel *labelMode;          //显示当前模式,执红执黑等信息
    QLabel *labelXia;           //该哪方下了


    QMenu *menuFile ;           //文件菜单
    QMenu *menuVier;            //视图菜单
    QMenu *menuOperator;        //操作菜单
    QMenu *menuHelp;            //帮助菜单
    QAction * actSets;          //打开设置对话框
    QAction *actCtrlPanel;      //打开控制面板
    QAction *actSave ;          //保存棋局
    QAction *actOpen;           //打开保存过的棋局
    QAction *actUndoStep;		//悔棋
    QAction *actStart;			//开始
    QAction *actRestart;		//重新开始
    QAction *actMagnify;		//棋盘放大
    QAction *actShrink;			//棋盘缩小
    QAction *actAdapt;			//棋盘适应窗口
    QAction *actGameRule;		//游戏规则
    QAction *actInstructions;	//软件使用说明
    QAction *actSelectMode;     //选择模式
    QAction *actQuit;			//退出

    //重新连接信号和槽,并设置中心部件
    void setBoard();

public slots:
 //   void slotAiMode();
    void slotSelect();
    void slotSetState(const QString str);
    void slotSetMode(const QString str);
    void slotCtrlPanel();
    void slotShowHelp();
};

#endif // MAINWINDOW_H
