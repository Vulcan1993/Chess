#ifndef RULE_H
#define RULE_H
#include"compconfig.h"

#include <QWidget>

namespace Ui {
class Rule;
}

class Rule : public QWidget
{
    Q_OBJECT

public:
    explicit Rule(QWidget *parent = 0);
    ~Rule();

private:
    Ui::Rule *ui;
};

#endif // RULE_H
