#include "paratable.h"

/*
struct tablePara {

    QVector<parameterBPSK> BPSKParaList;

    QVector<parameterBOC> BOCParaList;

};



*/

paraTable::paraTable(struct tablePara &globalTablePara) {

    BPSKNum = globalTablePara.BPSKParaList.size();
    BOCNum = globalTablePara.BOCParaList.size();

    BPSKList.resize(BPSKNum);
    BOCList.resize(BOCNum);

    for(int i=0;i<BPSKNum;i++) {

        //  构造 BPSK 数组  Fc 设置为  0   MHz !!!!!!!!!!!!!!!

        qDebug()<<globalTablePara.BPSKParaList[i].fb<<endl;


        BPSKList[i] = new bpsk(globalTablePara.BPSKParaList[i].fb,0);

    }

    for(int i=0;i<BOCNum;i++) {

        //  构造 BOC  数组

        BOCList[i] = new boc(globalTablePara.BOCParaList[i].alpha,globalTablePara.BOCParaList[i].beta);

    }

}

//  计算表格需要用到的参数

void paraTable::calculate() {

    for(int i=0;i<BPSKNum;i++) {

        BPSKList[i]->afterBetaPower();
        BPSKList[i]->calcuRMSBeta();

    }

    for(int i=0;i<BOCNum;i++) {

        BOCList[i]->afterBetaPower();
        BOCList[i]->calcuRMSBeta();

    }

}
