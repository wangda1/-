#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <QVector>


namespace sp{


    const double  PI  = 3.141592653589793;


    enum domainTypes{isTime,isFreq,isRela,isTable};

    const double MHz = 10E+5;
    const double KHz = 10E+2;
    const double Us = 1E-6;

    struct parameterBox{

        bool isChangedGlobal;

        //  控件显示的文本值
        int boxTBTimeSamNum;

        int boxfreqSamNum;
        int boxBPSKfreqBand;
        double boxBOCfreqBand;

        double boxrelaTotalTime;
        int boxrelaSamNum;
        double boxrBeta;

    };

    struct parameterBPSK {

        double fb;


    };

    struct parameterBOC {

        int alpha;

        int beta;

    };

    struct tablePara {

        QVector<parameterBPSK> BPSKParaList;

        QVector<parameterBOC> BOCParaList;

    };

    /*
    struct parameterGlobal {

        //  运算之后的实际参数
        bool isChangedGlobal;
        int TBTimeSamNumGlobal;

        unsigned long freqSamNumGlobal;
        int BPSKfreqBandGlobal;
        double BOCfreqBandGlobal;

        double relaTotalTimeGlobal;
        unsigned long relaTimeSamNumGlobal;
        double rBetaGlobal;

    };
    */

    extern struct parameterBox volatile globalBoxPara;
    //extern struct parameterGlobal volatile globalPara;

    extern struct tablePara tableParaGlobal;

}



#endif
