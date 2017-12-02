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

    QListWidget *leftlist;      //左边的列表
    QStackedWidget *stack;      //栈部件

    MultiPage *m;
    AiPage *a;
    NetPage *n;
signals:

public slots:
};

#endif // STACKED_H
