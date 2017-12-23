#ifndef PARATABLEWINDOW_H
#define PARATABLEWINDOW_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QtDebug>
#include "bpsk.h"
#include "boc.h"
#include "constants.h"
#include "toolfunc.h"

using namespace sp;

class paraTable {

public:

    paraTable(struct tablePara &globalTablePara);
    void calculate();


    QVector<bpsk *> BPSKList;
    QVector<boc *> BOCList;

    int BPSKNum;
    int BOCNum;

};




#endif // PARATABLEWINDOW_H
