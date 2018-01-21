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

    selectDialog *m_selectMode;         //选择对话框
    ControlPanel *m_panel;
    QDockWidget *m_dockWidget;
    Board *m_board;
    QLabel *m_labelMode;          //显示当前模式,执红执黑等信息
    QLabel *m_labelXia;           //该哪方下了


    QMenu *m_menuFile ;           //文件菜单
    QMenu *m_menuVier;            //视图菜单
    QMenu *m_menuOperator;        //操作菜单
    QMenu *m_menuHelp;            //帮助菜单
    QAction * m_actSets;          //打开设置对话框
    QAction *m_actCtrlPanel;      //打开控制面板
    QAction *m_actSave ;          //保存棋局
    QAction *m_actOpen;           //打开保存过的棋局
    QAction *m_actUndoStep;		//悔棋
    QAction *m_actStart;			//开始
    QAction *m_actRestart;		//重新开始
    QAction *m_actMagnify;		//棋盘放大
    QAction *m_actShrink;			//棋盘缩小
    QAction *m_actAdapt;			//棋盘适应窗口
    QAction *m_actGameRule;		//游戏规则
    QAction *m_actInstructions;	//软件使用说明
    QAction *m_actSelectMode;     //选择模式
    QAction *m_actQuit;			//退出

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
