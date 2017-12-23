#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawwindow.h"
#include "parawindow.h"
#include <QMessageBox>
#include <QtDebug>
#include "constants.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void plotTimeWaveForm();
    void plotFreqWaveForm();
    void plotRelaWaveForm();

private slots:

    void popDrawTimeWindow();       //时域波形图形窗
    void popDrawFreqWindow();       //频域波形图形窗
    void popDrawRelaWindow();       //自相关函数波形图形窗

    void popParaWindow();           //参数设置窗口
    //void popEvenWarnningWindow();       //  BOC 非偶数警告框

    void popTableWindow();

    void showProgress(double value);

signals:

    void drawPara(bool bpsk=false,double fb=0.0,double fc=0.0,bool boc=false,int alpha=0,int beta=0.0);

private:
    Ui::MainWindow *ui;
    void InitConnect();
    void InitUI();
    void clearProgressBar();
    void fullProgressBar();
    //drawWindow timeDrawWindow;
    //drawWindow *freqDrawWindow;
    //drawWindow *relaDrawWindow;

};


#endif // MAINWINDOW_H
