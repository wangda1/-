#ifndef PARAWINDOW_H
#define PARAWINDOW_H

#include <QWidget>
#include "constants.h"

namespace Ui {
class paraWindow;
}

class paraWindow : public QWidget
{
    Q_OBJECT

public:
    explicit paraWindow(QWidget *parent = 0);
    ~paraWindow();

private slots:

    void recoveryDefaultPara();
    void savePara();


private:

    void recoverySavePara();
    void InitUI();
    void InitConnect();
    Ui::paraWindow *ui;
};

#endif // PARAWINDOW_H
