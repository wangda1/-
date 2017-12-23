#include "drawwindow.h"
#include "ui_drawwindow.h"

/*  Bug1:   计算自相关函数需要先计算出频谱！！！
 *  功能：     保存成PDF（单页）仅能保存成PDF
 *            Tab Widget 添加
 *  画图：     坐标设置，单位设置，窗口设置，坐标轴设置
 *
 */



drawWindow::drawWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::drawWindow)
{
    ui->setupUi(this);

    //InitConnect();        加入表格后，初始化连接放到 绘制函数里面


    //BPSK = new bpsk(bpskFb,bpskFc);
    //BOC = new boc(bocAlpha,bocBeta);

}

//  重载构造函数，传递参数，并进行运算
drawWindow::drawWindow(domainTypes _type,  bool _bpskCheck, bool _bocCheck, double _bpskFc,
                       double _bpskFb, int _bocAlpha, int _bocBeta,QWidget *parent) :
                        QMainWindow(parent),
                        ui(new Ui::drawWindow)
{
    ui->setupUi(this);

    type = _type;

    bpskCheck = _bpskCheck;
    bpskFc = _bpskFc * 10E+5;
    bpskFb = _bpskFb * 10E+5;
    bocCheck = _bocCheck;
    bocAlpha = _bocAlpha;
    bocBeta = _bocBeta;

    if(globalBoxPara.isChangedGlobal && bpskCheck) {

        BPSK = new bpsk(bpskFb,bpskFc,
                        globalBoxPara.boxTBTimeSamNum,
                        globalBoxPara.boxfreqSamNum * KHz + 1,
                        globalBoxPara.boxBPSKfreqBand,
                        globalBoxPara.boxrelaTotalTime * Us,
                        globalBoxPara.boxrelaSamNum * KHz,
                        globalBoxPara.boxrBeta * MHz / 2);

    }
    else if(bpskCheck)
        BPSK = new bpsk(bpskFb,bpskFc);

    if(globalBoxPara.isChangedGlobal && bocCheck) {

        BOC = new boc(bocAlpha,bocBeta,
                      globalBoxPara.boxTBTimeSamNum,
                      globalBoxPara.boxfreqSamNum * KHz + 1,
                      globalBoxPara.boxBOCfreqBand * MHz,
                      globalBoxPara.boxrelaTotalTime * Us,
                      globalBoxPara.boxrelaSamNum * KHz,
                      globalBoxPara.boxrBeta * MHz /2);
    }
    else if(bocCheck)
        BOC = new boc(bocAlpha,bocBeta);


    InitConnect();

}

drawWindow::~drawWindow()
{
    delete ui;
    delete BPSK;
    delete BOC;
    delete plot;
}

void drawWindow::calu() {

    qDebug()<<"In calu Box"<<globalBoxPara.isChangedGlobal<<endl;

    //qDebug()<<"In calu Para"<<globalPara.isChangedGlobal<<endl;

    if(bpskCheck) {

        switch (type) {
            case isTime:
            {
                BPSK->timeDomain();
                break;
            }
            case isFreq:
            {
                BPSK->freqDomain();
                break;
            }
            case isRela:
            {
                //BPSK->freqDomain();
                BPSK->autocorrelation();
                break;
            }
            default:
                break;
        }
    }

    if(bocCheck) {

        switch (type) {
            case isTime:
            {
                BOC->timeDomain();
                break;
            }
            case isFreq:
            {
                BOC->freqDomain();
                break;
            }
            case isRela:
            {
                //BOC->freqDomain();
                BOC->autocorrelation();
                break;
            }
            default:
                break;
        }
    }
}

/*  绘制函数
 *  功能：满足在一张图形中绘制两幅图的功能
 *       添加图例，坐标，
 *
 *
 */
void drawWindow::plotgraph() {

    isGraph = true;
    InitConnect();

    calu();         //对数据进行计算，
    plot = new QCustomPlot(this);
    ui->gridLayout->addWidget(plot,0,0,1,1);

    QCPTextElement *title = new QCPTextElement(plot);

    title->setFont(QFont("Adobe 楷体 Std R"));
    plot->plotLayout()->addElement(0,0,title);

    //  仅绘制 BPSK 信号图形
    if(bpskCheck && !bocCheck) {

        plot->addGraph();

        switch (type) {
        case isTime: {

            title->setText("BPSK时域波形");

            plot->xAxis->setLabel("t(s)");
            plot->yAxis->setLabel("幅值(V)");

            plot->graph(0)->setLineStyle(QCPGraph::lsImpulse);
            plot->graph(0)->setData(BPSK->timeSample,BPSK->timeWaveForm);
            plot->xAxis->setRange(BPSK->timeSample[0],BPSK->timeSample[BPSK->timeSamNum-1]);
            plot->yAxis->setRange(findMin(BPSK->timeWaveForm),findMax(BPSK->timeWaveForm));

            break;
        }
        case isFreq: {

            title->setText("BPSK频域波形");

            plot->xAxis->setLabel("f(Hz)");
            plot->yAxis->setLabel("W/Hz");

            plot->graph(0)->setData(BPSK->freqSample,BPSK->freqWaveForm);
            plot->xAxis->setRange(BPSK->freqSample[0],BPSK->freqSample[BPSK->freqSamNum-1]);
            plot->yAxis->setRange(findMin(BPSK->freqWaveForm),findMax(BPSK->freqWaveForm));

            break;
        }
        case isRela: {

            title->setText("BPSK自相关波形");

            plot->xAxis->setLabel("时间T(s)");
            plot->yAxis->setLabel("(V)");

            plot->graph(0)->setData(BPSK->relaSample,BPSK->normalAbsRelaWaveForm);
            plot->xAxis->setRange(BPSK->relaSample[0],BPSK->relaSample[BPSK->relaTimeSamNum-1]);
            plot->yAxis->setRange(findMin(BPSK->normalAbsRelaWaveForm),findMax(BPSK->normalAbsRelaWaveForm));

            break;

        }
        default:
            break;
        }

    }
    //  仅绘制 BOC信号图形
    else if(bocCheck && !bpskCheck) {

        plot->addGraph();

        switch (type) {
        case isTime: {
            title->setText("BOC时域波形");

            plot->xAxis->setLabel("t(s)");
            plot->yAxis->setLabel("幅值(V)");

            plot->graph(0)->setLineStyle(QCPGraph::lsImpulse);
            plot->graph(0)->setData(BOC->timeSample,BOC->timeWaveForm);
            plot->xAxis->setRange(BOC->timeSample[0],BOC->timeSample[BOC->timeSamNum-1]);
            plot->yAxis->setRange(findMin(BOC->timeWaveForm),findMax(BOC->timeWaveForm));

            break;
        }
        case isFreq: {
            title->setText("BOC频域波形");

            plot->xAxis->setLabel("f(Hz)");
            plot->yAxis->setLabel("W/Hz");

            plot->graph(0)->setData(BOC->freqSample,BOC->freqDBWaveForm);
            plot->xAxis->setRange(BOC->freqSample[0],BOC->freqSample[BOC->freqSamNum-1]);
            plot->yAxis->setRange(findMin(BOC->freqDBWaveForm),findMax(BOC->freqDBWaveForm));

            break;
        }
        case isRela: {
            title->setText("BOC自相关波形");

            plot->xAxis->setLabel("时间T(s)");
            plot->yAxis->setLabel("(V)");

            plot->graph(0)->setData(BOC->relaSample,BOC->normalAbsRelaWaveForm);
            plot->xAxis->setRange(BOC->relaSample[0],BOC->relaSample[BOC->relaTimeSamNum-1]);
            plot->yAxis->setRange(findMin(BOC->normalAbsRelaWaveForm),findMax(BOC->normalAbsRelaWaveForm));

            break;

        }
        default:
            break;
        }

    }
    //  绘制 BPSK BOC 两种图形
    else
        return;
        /*
        plot->addGraph();           //  BPSK
        plot->addGraph();           //  BOC

        switch (type) {
        case isTime: {
            plot->graph(0)->setData(BPSK->timeSample,BPSK->timeWaveForm);
            plot->xAxis->setRange(findMin(BPSK->timeSample),findMax(BPSK->timeSample));
            plot->yAxis->setRange(findMin(BPSK->timeWaveForm),findMax(BPSK->timeWaveForm));

            break;
        }
        case isFreq: {
            plot->graph(0)->setData(BPSK->freqSample,BPSK->freqWaveForm);
            plot->xAxis->setRange(findMin(BPSK->freqSample),findMax(BPSK->freqSample));
            plot->yAxis->setRange(findMin(BPSK->freqWaveForm),findMax(BPSK->freqWaveForm));

            break;
        }
        case isRela: {
            plot->graph(0)->setData(BPSK->relaSample,BPSK->normalAbsRelaWaveForm);
            plot->xAxis->setRange(findMin(BPSK->relaSample),findMax(BPSK->relaSample));
            plot->yAxis->setRange(findMin(BPSK->normalAbsRelaWaveForm),findMax(BPSK->normalAbsRelaWaveForm));

            break;

        }
        default:
            break;
        }
        */

    plot->replot();

}


void drawWindow::InitConnect() {

    if(isGraph)
        QObject::connect(ui->printAction,SIGNAL(triggered(bool)),this,SLOT(savePDF()));
    else
        QObject::connect(ui->printAction,SIGNAL(triggered(bool)),this,SLOT(saveTable()));

}

void drawWindow::savePDF() {

    QString fileName = QFileDialog::getSaveFileName(this,"Open file",".","PDF Files(.pdf)");

    qDebug()<<fileName<<endl;

    if(!fileName.isEmpty()) {

        if(QFileInfo(fileName).suffix().isEmpty()) {

            fileName.append(".pdf");
        }
    }

    plot->savePdf(fileName);

    //saveFile = new QFile()


}

/*  绘制表格：材料 Page12，
 *  表格样式：   rows 12 * columns 6
 *  tableParaGlobal 为表格全局参数
 *
 *
 */
void drawWindow::plotTable() {

    isTable = true;
    InitConnect();

    //  数据的计算
    TablePara = new paraTable(tableParaGlobal);
    TablePara->calculate();

    //  表格的绘制
    table = new QTableWidget(ui->centralWidget);
    //  布局
    ui->gridLayout->addWidget(table,0,0,1,1);

    qDebug()<<"centralWidget Width:"<<centralWidget()->width()<<endl;
    qDebug()<<"centralWidget height:"<<centralWidget()->height()<<endl;
    qDebug()<<"tableWidget is"<<table->width()<<endl;
    qDebug()<<"tableWidget is"<<table->height()<<endl;

    //this->setMaximumWidth(table->width());
    //this->setMaximumHeight(table->height());

    table->setRowCount(11);
    table->setColumnCount(TablePara->BPSKNum +TablePara->BOCNum);

    /*
    QVector<QTableWidgetItem *> firstRowItemList = {new QTableWidgetItem("特性"),
                                                   new QTableWidgetItem("1.023MHz BPSK"),
                                                   new QTableWidgetItem("10.23MHz BPSK"),
                                                   new QTableWidgetItem("BOC(5,2)"),
                                                   new QTableWidgetItem("BOC(8,4)"),
                                                   new QTableWidgetItem("BOC(10,5)")
                                                 };

    QVector<QTableWidgetItem> firstColumnList = {  QTableWidgetItem("频谱主瓣距频带中心的频偏(MHz)"),
                                                   QTableWidgetItem("主瓣最大功率谱密度(dBW/Hz)"),
                                                   QTableWidgetItem("90%功率的带宽(MHz)"),
                                                   QTableWidgetItem("带外的损失(dB)"),
                                                   QTableWidgetItem("RMS带宽(MHz)"),
                                                   QTableWidgetItem("等效矩形带宽(MHz)"),
                                                   QTableWidgetItem("与自身的频谱隔离系数(dB/Hz)"),
                                                   QTableWidgetItem("与1.023MHzBPSK的频谱隔离系数(dB/Hz)"),
                                                   QTableWidgetItem("与BOC(10,5)的频谱隔离系数(dB/Hz)"),
                                                   QTableWidgetItem("自相关函数主峰与第一副峰间的时延(ns)"),
                                                   QTableWidgetItem("自相关函数第一副峰与主峰幅度平方之比")
                                                 };

    */

    QStringList rowHeader;
    rowHeader<<"1.023MHz BPSK"<<"10.23MHz BPSK"<<"BOC(5,2)"<<"BOC(8,4)"<<"BOC(10,5)";

    table->setHorizontalHeaderLabels(rowHeader);

    QStringList columnHeader;
    columnHeader<<"频谱主瓣距频带中心的频偏(MHz)"<<"主瓣最大功率谱密度(dBW/Hz)"<<"90%功率的带宽(MHz)"
               <<"带外的损失(dB)"<<"RMS带宽(MHz)"<<"等效矩形带宽(MHz)"<<"与自身的频谱隔离系数(dB/Hz)"
              <<"与1.023MHzBPSK的频谱隔离系数(dB/Hz)"<<"与BOC(10,5)的频谱隔离系数(dB/Hz)"<<"自相关函数主峰与第一副峰间的时延(ns)"
             <<"自相关函数第一副峰与主峰幅度平方之比";

    table->setVerticalHeaderLabels(columnHeader);

    table->show();

}

void drawWindow::saveTable() {

    QString tableName = QFileDialog::getSaveFileName(this,"Open File",".","Image(.jpg)");

    if(QFileInfo(tableName).suffix().isEmpty()) {

        tableName.append(".jpg");
    }

    QPixmap pix = QPixmap::grabWidget(table);

    pix.save(tableName);

}
