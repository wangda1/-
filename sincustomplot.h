#ifndef SINCUSTOMPLOT_H
#define SINCUSTOMPLOT_H
#include "qcustomplot.h"


class sinCustomPlot:public QCustomPlot {
public:
    sinCustomPlot(QWidget *parent,QVector<double> &x,QVector<double> &y,int xLength,int yLength):
        QCustomPlot(parent) {
        _x = x;
        _y = y;
        _xLength = xLength;
        _yLength = yLength;
    }
    void sinPlot();
    void cosPlot();

private:
    QVector<double> _x;
    QVector<double> _y;
    int _xLength;
    int _yLength;
};

inline void sinCustomPlot::sinPlot() {

    this->addGraph();
    this->graph(0)->setData(_x,_y);

    this->xAxis->setRange(-1,11);
    this->yAxis->setRange(-1,1);

    this->xAxis->setLabel("X");
    this->yAxis->setLabel("Y");

}
#endif // SINCUSTOMPLOT_H
