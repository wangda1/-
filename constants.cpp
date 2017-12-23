#include "constants.h"


  /*    1.在这里为了保存修改之后的全局参数变量，且这些都是默认参量
   *    2.默认参数的修改 这里要修改   3   处，使这三处保持一致，分别是 constans.h   bpsk.h  boc.h   中的构造函数
   *    3.表格默认参数的修改：      2   处，这两处保持一致，分别是   constans.h    drawwindow.cpp    中的表头项目
   *    4.测试最佳默认取点参数：
   */
namespace sp {

    //  全局绘图参数定义

    struct parameterBox volatile globalBoxPara = {

        .isChangedGlobal = false,                      //  bool  isChangedGlobal
        .boxTBTimeSamNum = 20,                        //  int   TBTimeSamNumGlobal
        .boxfreqSamNum = 10,                          //   unsigned long int freqSamNumGlobal
        .boxBPSKfreqBand = 4,                          //  int BPSKfreqBandGlobal
        .boxBOCfreqBand = 30.00,                      //  double BOCfreqBandGloba
        .boxrelaTotalTime = 2.00,                     //  double relaTotalTimeGlobal
        .boxrelaSamNum = 1,                           //  unsigned long int relaTimeSamNumGlobal
        .boxrBeta = 24.00,                            //  double rBetaGlobal

    };

    //  全局表格参数定义

    tablePara tableParaGlobal = {

        .BPSKParaList = { { .fb = 1.023 * MHz },
                      { .fb = 10.23 * MHz }
                    },

        .BOCParaList = { {.alpha = 5,.beta = 2},
                     {.alpha = 8,.beta = 4},
                     {.alpha = 10,.beta = 5}
                    }
    };

    /*
    struct parameterGlobal volatile globalPara ={


         .isChangedGlobal = globalBoxPara.isChangedGlobal,

         .TBTimeSamNumGlobal = globalBoxPara.boxTBTimeSamNum,

         .freqSamNumGlobal = globalBoxPara.boxfreqSamNum * KHz +1,
         .BPSKfreqBandGlobal = globalBoxPara.boxBPSKfreqBand,
         .BOCfreqBandGlobal = globalBoxPara.boxBOCfreqBand * MHz,

         .relaTotalTimeGlobal = globalBoxPara.boxrelaTotalTime * Us,
         .relaTimeSamNumGlobal = globalBoxPara.boxrelaSamNum * KHz,
         .rBetaGlobal = globalBoxPara.boxrBeta * MHz / 2

    };
    */

}




