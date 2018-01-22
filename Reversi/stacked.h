#ifndef STACKED_H
#define STACKED_H

#include <QWidget>
#include<QListWidget>
#include<QStackedWidget>
#include<QLabel>
#include<QLayout>

#include"stackedpage.h"
class stacked : public QWidget
{
    Q_OBJECT
public:
    explicit stacked(QWidget *parent = 0);

    QListWidget *m_leftlist;      //左边的列表
    QStackedWidget *m_stack;      //栈部件

    MultiPage *m_multiPage;
    AiPage *m_aiPage;
    NetPage *m_netPage;
signals:

public slots:
};

#endif // STACKED_H
