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
    stacked *m_socket;
    QImage m_image;          //加载背景图片
    QPushButton *m_btn1;
    QPushButton *m_btn2;
    QPushButton *m_btn3;
    QPushButton *m_btn4;
private slots:
    void slot_select();
};









#if 0

class selectDialog
{
public:
    selectDialog();
};
#endif
#endif // selectDialog_H
