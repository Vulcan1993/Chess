#ifndef selectDialog_H
#define selectDialog_H

#include <QWidget>
#include<QPushButton>
#include<QDialog>
#include<QBoxLayout>
#include"stacked.h"
#include"base.h"

class selectDialog : public QDialog
{
    Q_OBJECT
public:
    selectDialog( QWidget *parent = 0, Qt::WindowFlags f = 0);

  //  selectDialog(mytest*base = nullptr,QWidget *parent = 0, Qt::WindowFlags f = 0);
    virtual ~selectDialog();
protected:
    void resizeEvent(QResizeEvent *event);
private:
    stacked *s;
    QImage _image;          //加载背景图片
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;
private slots:
    void slotSelect();
};









#if 0

class selectDialog
{
public:
    selectDialog();
};
#endif
#endif // selectDialog_H
