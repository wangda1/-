#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "drawwindow.h"

/*************************************
 * 功能：主窗口
 * QDoubleSpinBox:   ui->fbSpinBox;  ui->fcSpinBox;
 * QSpinBox:         ui->alphaSpinBox;  ui->betaSpinBox;
 * QButton:          ui->timeButton;    ui->freqButton;    ui->relaButton;
 * QLabel:           ui->fbLabel;       ui->fcLabel;       ui->alphaLabel;
 *                   ui->betaLabel;  ui->bocTitle;   ui->bpskTitle;     ui->unitFbLabel;       ui->unitFcLabel;
 * QMenu:            ui->optionMenu;    ui->helpMenu;
 * QAction:          ui->paraAction;
 * QCheckBox:        ui->bpskCheckBox;  ui->bocCheckBox;
 * QProgressBar:     ui->progressBar
 * 12/15
 * ***********************************/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitUI();
    InitConnect();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitConnect() {
    // 检查警告框
    QObject::connect(ui->timeButton,SIGNAL(clicked(bool)),this,SLOT(popDrawTimeWindow()));
    QObject::connect(ui->freqButton,SIGNAL(clicked(bool)),this,SLOT(popDrawFreqWindow()));
    QObject::connect(ui->relaButton,SIGNAL(clicked(bool)),this,SLOT(popDrawRelaWindow()));
    QObject::connect(ui->paraAction,SIGNAL(triggered(bool)),this,SLOT(popParaWindow()));
    QObject::connect(ui->paraTableButton,SIGNAL(clicked(bool)),this,SLOT(popTableWindow()));


}

void MainWindow::InitUI() {

    //  QSpinBox 默认最大值为 99.99，设置成  999.99
    ui->fbSpinBox->setMaximum(999.99);
    ui->fcSpinBox->setMaximum(999.99);

    ui->fcSpinBox->setDecimals(3);
    ui->fcSpinBox->setSingleStep(0.001);
    ui->fbSpinBox->setDecimals(3);
    ui->fbSpinBox->setSingleStep(0.001);

    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(1);

}

void MainWindow::popDrawTimeWindow() {

    clearProgressBar();

    if(ui->bpskCheckBox->isChecked()) {
        if(!ui->fbSpinBox->value()) {                            //     || !ui->fcSpinBox->value()
            QMessageBox::warning(this,"Warning","参数为0!!");
            return;
        }
    }
    if(ui->bocCheckBox->isChecked()) {
        if(!ui->alphaSpinBox->value() || !ui->betaSpinBox->value()) {
            QMessageBox::warning(this,"Warning","参数为0!!");
            return;
        }
        else if(2*(ui->alphaSpinBox->value()) % ui->betaSpinBox->value()) {
            QMessageBox::information(this,"Tips","参数不正确！");
            return;
        }
    }
    if(!ui->bpskCheckBox->isChecked() && !ui->bocCheckBox->isChecked()){
        QMessageBox::warning(this,"Warning","没有勾选！");
        return;
    }
    if(ui->bpskCheckBox->isChecked() && ui->bocCheckBox->isChecked()) {
        QMessageBox::warning(this,"Warning","请选择一个！");
        return;
    }

    drawWindow *timeWindow = new drawWindow(isTime,
                                            ui->bpskCheckBox->isChecked(),
                                            ui->bocCheckBox->isChecked(),
                                            ui->fcSpinBox->value(),
                                            ui->fbSpinBox->value(),
                                            ui->alphaSpinBox->value(),
                                            ui->betaSpinBox->value(),
                                            this);


    timeWindow->plotgraph();
    timeWindow->show();

    clearProgressBar();

}

void MainWindow::popDrawFreqWindow() {

    clearProgressBar();

    if(ui->bpskCheckBox->isChecked()) {
        if(!ui->fbSpinBox->value()) {
            QMessageBox::warning(this,"Warning","参数为0!!");
            return;
        }
    }
    if(ui->bocCheckBox->isChecked()) {
        if(!ui->alphaSpinBox->value() || !ui->betaSpinBox->value()) {
            QMessageBox::warning(this,"Warning","参数为0!!");
            return;
        }
        else if(2*(ui->alphaSpinBox->value()) % ui->betaSpinBox->value()) {
            QMessageBox::information(this,"Tips","参数不正确！");
            return;
        }
    }
    if(!ui->bpskCheckBox->isChecked() && !ui->bocCheckBox->isChecked()){
        QMessageBox::warning(this,"Warning","没有勾选！");
        return;
    }
    if(ui->bpskCheckBox->isChecked() && ui->bocCheckBox->isChecked()) {
        QMessageBox::warning(this,"Warning","请选择一个！");
        return;
    }

    drawWindow *freqWindow = new drawWindow(isFreq,
                                            ui->bpskCheckBox->isChecked(),
                                            ui->bocCheckBox->isChecked(),
                                            ui->fcSpinBox->value(),
                                            ui->fbSpinBox->value(),
                                            ui->alphaSpinBox->value(),
                                            ui->betaSpinBox->value(),
                                            this);
    freqWindow->plotgraph();
    freqWindow->show();

    clearProgressBar();

}

void MainWindow::popDrawRelaWindow() {

    clearProgressBar();

    if(ui->bpskCheckBox->isChecked()) {
        if(!ui->fbSpinBox->value()) {
            QMessageBox::warning(this,"Warning","参数为0!!");
            return;
        }
    }
    if(ui->bocCheckBox->isChecked()) {
        if(!ui->alphaSpinBox->value() || !ui->betaSpinBox->value()) {
            QMessageBox::warning(this,"Warning","参数为0!!");
            return;
        }
        else if(2*(ui->alphaSpinBox->value()) % ui->betaSpinBox->value()) {
            QMessageBox::information(this,"Tips","参数不正确！");
            return;
        }
    }
    if(!ui->bpskCheckBox->isChecked() && !ui->bocCheckBox->isChecked()){
        QMessageBox::warning(this,"Warning","没有勾选！");
        return;
    }
    if(ui->bpskCheckBox->isChecked() && ui->bocCheckBox->isChecked()) {
        QMessageBox::warning(this,"Warning","请选择一个！");
        return;
    }

    drawWindow *relaWindow = new drawWindow(isRela,
                                            ui->bpskCheckBox->isChecked(),
                                            ui->bocCheckBox->isChecked(),
                                            ui->fcSpinBox->value(),
                                            ui->fbSpinBox->value(),
                                            ui->alphaSpinBox->value(),
                                            ui->betaSpinBox->value(),
                                            this);

    if(ui->bpskCheckBox->isChecked())
        QObject::connect(relaWindow->BPSK,SIGNAL(sendProgress(double)),this,SLOT(showProgress(double)));
    else if(ui->bocCheckBox->isChecked())
            QObject::connect(relaWindow->BOC,SIGNAL(sendProgress(double)),this,SLOT(showProgress(double)));

    relaWindow->plotgraph();
    relaWindow->show();

    clearProgressBar();


}

void MainWindow::popParaWindow() {

    clearProgressBar();

    paraWindow *paraOption = new paraWindow();

    paraOption->show();
}

/*  功能： 参数表格展示
 *  展示在 drawParaWindow 窗口中，
 *  需要加入：   参数检查， BPSK,BOC参数都应该输入正确
 *
 */
void MainWindow::popTableWindow() {


    drawWindow *paraTabelWindow = new drawWindow(this);


    paraTabelWindow->plotTable();
    paraTabelWindow->show();

}

/*  槽函数：显示进度条进度
 *  参数： double value   表示此时为 0-1之间的显示进度
 */

void MainWindow::showProgress(double value) {

    ui->progressBar->setValue(value * ui->progressBar->maximum());

}

/*  每次点击按钮后清空进度条
 *
 */

void MainWindow::clearProgressBar() {


    ui->progressBar->setValue(1);


}

/*  绘制之前打满进度条
 */
void MainWindow::fullProgressBar() {


    ui->progressBar->setValue(100);
}


