#include "parawindow.h"
#include "ui_parawindow.h"

/*  功能 Tab 窗口实现参数设置
 *  Time:   ui->TBTimeSamNumSpinBox;
 *  Freq:   ui->freqSamNumSpinBox;   ui->BOCfreqBandSpinBox;   ui->BPSKfreqBandSpinBox;
 *  Rela:   ui->rBetaSpinBox;       ui->relaTimeSamNumSpinBox;  ui->relaTotalTimeSpinBox;
 *  Button:     ui->saveButton;     ui->recoveryDefaultButton;
 *
 *
 */

//const int MHz = 10E+5;
//const int KHz = 10E+2;

using sp::KHz;
using sp::MHz;
using sp::globalBoxPara;
using sp::Us;


paraWindow::paraWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::paraWindow)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();

}

paraWindow::~paraWindow()
{
    delete ui;
}



void paraWindow::InitConnect() {

    QObject::connect(ui->recoveryDefaultButton,SIGNAL(clicked(bool)),this,SLOT(recoveryDefaultPara()));

    QObject::connect(ui->saveButton,SIGNAL(clicked(bool)),this,SLOT(savePara()));


}

void paraWindow::InitUI() {

    //  设置最大参数

    ui->TBTimeSamNumSpinBox->setMaximum(1000);  //个

    ui->freqSamNumSpinBox->setMaximum(1000);    //  K + 1
    ui->BPSKfreqBandSpinBox->setMaximum(10);    //为偶数   10 * fb
    ui->BOCfreqBandSpinBox->setMaximum(100.00);     //  MHz

    ui->relaTotalTimeSpinBox->setMaximum(10.00);    //  us
    ui->relaTimeSamNumSpinBox->setMaximum(10);  //  K
    ui->rBetaSpinBox->setMaximum(24.00);

    recoverySavePara();

}

void paraWindow::recoveryDefaultPara() {

    //  设置默认参数

    ui->TBTimeSamNumSpinBox->setValue(20);

    ui->freqSamNumSpinBox->setValue(10);
    ui->BPSKfreqBandSpinBox->setValue(4);
    ui->BOCfreqBandSpinBox->setValue(30.00);

    ui->relaTotalTimeSpinBox->setValue(2.00);
    ui->relaTimeSamNumSpinBox->setValue(1);
    ui->rBetaSpinBox->setValue(24.00);

    globalBoxPara.isChangedGlobal = false;         //默认参数没有发生改变
    savePara();

}

//  恢复成已经设置参数
void paraWindow::recoverySavePara() {

    ui->TBTimeSamNumSpinBox->setValue(globalBoxPara.boxTBTimeSamNum);

    ui->freqSamNumSpinBox->setValue(globalBoxPara.boxfreqSamNum);
    ui->BPSKfreqBandSpinBox->setValue(globalBoxPara.boxBPSKfreqBand);
    ui->BOCfreqBandSpinBox->setValue(globalBoxPara.boxBOCfreqBand);

    ui->relaTotalTimeSpinBox->setValue(globalBoxPara.boxrelaTotalTime);
    ui->relaTimeSamNumSpinBox->setValue(globalBoxPara.boxrelaSamNum);
    ui->rBetaSpinBox->setValue(globalBoxPara.boxrBeta);

}

void paraWindow::savePara() {

    globalBoxPara.isChangedGlobal = true;          //默认参数发生改变

    globalBoxPara.boxTBTimeSamNum = ui->TBTimeSamNumSpinBox->value();

    globalBoxPara.boxfreqSamNum = ui->freqSamNumSpinBox->value();
    globalBoxPara.boxBPSKfreqBand = ui->BPSKfreqBandSpinBox->value();
    globalBoxPara.boxBOCfreqBand = ui->BOCfreqBandSpinBox->value();

    globalBoxPara.boxrelaTotalTime = ui->relaTotalTimeSpinBox->value();
    globalBoxPara.boxrelaSamNum = ui->relaTimeSamNumSpinBox->value();
    globalBoxPara.boxrBeta = ui->rBetaSpinBox->value();

    this->close();

}
