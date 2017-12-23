#include "sincustomplot.h"

void sinCustomPlot::cosPlot() {
    this->addGraph();
    this->graph(0)->setData(_x,_y);

    this->xAxis->setRange(-1,11);
    this->yAxis->setRange(-1,1);

    this->xAxis->setLabel("X");
    this->yAxis->setLabel("Y");
}
