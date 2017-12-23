#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QFont>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QtDebug>
#include "bpsk.h"
#include "boc.h"
#include "paratable.h"
#include "constants.h"
#include "qcustomplot.h"
#include "toolfunc.h"


using namespace sp;

namespace Ui {
class drawWindow;
}

class drawWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit drawWindow(QWidget *parent = 0);
    explicit drawWindow(domainTypes _type,bool _bpskCheck,bool _bocCheck,double bpskFc,
                        double _bpskFb,int _bocAlpha,int _bocBeta,QWidget *parent = 0);
    ~drawWindow();
    void plotgraph();
    void plotTable();

private slots:
    void savePDF();
    void saveTable();

signals:
    void progressNum(int value);

private:

    void InitConnect();
    void calu();


    QFile *saveFile;
    QCustomPlot *plot;
    QTableWidget *table;
    domainTypes type;           //  绘制图形类型

    bool bpskCheck;
    bool bocCheck;

    double bpskFc;
    double bpskFb;
    int bocAlpha;
    int bocBeta;

    bool isGraph = false;
    bool isTable = false;

    Ui::drawWindow *ui;

public:
    bpsk *BPSK;
    boc *BOC;
    paraTable *TablePara;

};

#endif // DRAWWINDOW_H
