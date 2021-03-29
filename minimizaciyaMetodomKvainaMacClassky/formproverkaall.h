#ifndef FORMPROVERKAALL_H
#define FORMPROVERKAALL_H

#include <QWidget>

namespace Ui {
class formProverkaAll;
}

class formProverkaAll : public QWidget
{
    Q_OBJECT

public:
    explicit formProverkaAll(QWidget *parent = nullptr);
    ~formProverkaAll();
    void setData(bool tableOfTrue = false, bool viborMin = false, bool skleyka1 = false, bool skleyka2 = false, bool skleyka3 = false, bool kartaPokritiya = false, bool itogMnf = false, QString levelOfhard = "");

private:
    Ui::formProverkaAll *ui;
};

#endif // FORMPROVERKAALL_H
