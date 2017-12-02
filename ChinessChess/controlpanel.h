#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H
#include"compconfig.h"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>


class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent = 0);
public:
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
    QPushButton *btn5;
    QPushButton *btn6;
    QPushButton *btn7;
    QPushButton *btn8;



signals:

public slots:
};

#endif // CONTROLPANEL_H
