#include "bpsk.h"
#include <cmath>
#include <complex>
#include "constants.h"
#include "toolfunc.h"

#include <iostream>
using std::cout;
using std::endl;

using std::cos;
using std::sin;
using std::fabs;
using std::pow;
using std::abs;
using std::exp;
using std::sqrt;

/*C++11 版本没有定义  i   ,而在C++ 14实现了该定义，
 * 定义于内联命名空间   std::complex::literals
 */

using std::complex;

namespace sp {

    /*  公式为 cos(Wc*t)*a[i]
     *
     *
     *
     *
     *
     *
     */
    bpsk::~bpsk() {

    }

    void bpsk::timeDomain() {

        //const int timeSamNumTb = 20;
        const double interval = Tb / timeSamNumTb;
        const double Wc = 2 * PI * fc;

        timeSample.resize(timeSamNum);                 //对应的时间点 每个Tb设置20个采样点
        timeWaveForm.resize(timeSamNum);              //对应的波形点

        for (int i = 0; i < length; i++) {             //在每个Tb内进行计算，减小误差

            for (int t = 0; t < timeSamNumTb; t++) {         //每个Tb内的20个取样点

                timeSample[i * timeSamNumTb + t] = i * Tb + t * interval;

                timeWaveForm[i * timeSamNumTb + t] = cos(Wc * timeSample[i * timeSamNumTb + t])*a[i];
            }

        }

    }
    /*  公式为 详见课本
     *
     *
     *
     *
     *
     *
     */

    void bpsk::freqDomain() {

        //const double fb = 1/Tb;                                 //  取 fc-2fb ---- fc+2fb内的频率点
        //const double interFreq = 4 * fb / (freqSamNum - 1);          //  取   4个 fb的频率范围

        if(hasCalcuFreq)
            return;

        const double interFreq = freqBand / (freqSamNum - 1);     //  取   4个 fb的频率范围
        double f;

        freqSample.resize(freqSamNum);
        freqWaveForm.resize(freqSamNum);
        freqDBWaveForm.resize(freqSamNum);

        for(long long int i=(freqSamNum - 1)/2;i>=0;i--) {                    //  以 fc 为中间两边取 40个点

            freqSample[i] = f = fc - ((freqSamNum - 1) / 2 - i) * interFreq;      //向两端扩散

            if(f == fc)
                freqSample[i] = f = f + 1E-308;                                //  数据为 0
            qDebug()<<"Double"<<sizeof(double)<<endl;


            freqWaveForm[i] = Tb / 4 * (pow(fabs(sin(PI * (f + fc) * Tb) / (PI * (f + fc) * Tb)), 2.0) +
                                        pow(fabs(sin(PI * (f - fc) * Tb) / (PI * (f - fc) * Tb)), 2.0));
            freqDBWaveForm[i] = 10*log10(freqWaveForm[i]);

        }
        for(unsigned long i=(freqSamNum - 1)/2 + 1;i<freqSamNum;i++) {

            freqSample[i] = f = fc + (i - (freqSamNum - 1) / 2) * interFreq;      //向两端扩散

            if(f == fc)
                freqSample[i] = f = f + 1E-308;                                        //  数据为 0

            freqWaveForm[i] = Tb / 4 * (pow(fabs(sin(PI * (f + fc) * Tb) / (PI * (f + fc) * Tb)), 2) +
                                        pow(fabs(sin(PI * (f - fc) * Tb) / (PI * (f - fc) * Tb)), 2));

            freqDBWaveForm[i] = 10*log10(freqWaveForm[i]);

        }

        hasCalcuFreq = true;
    }

    /*  自相关函数计算，对频谱进行傅里叶逆变换
     *  公式为 P(f)*exp(2*PI*f*t)
     *  进行求和运算代替积分运算
     *  12/22   优化 不用 complex 改为直接相加求和
     *  12/23   优化，rBeta，为  24 MHz，不适用 BPSK 情况， rBeta取所采样点的带宽
     *
     */

    /*
    void bpsk::autocorrelation() {

        //  relatotalTime       指的是自相关函数计算的时间范围，这里以 2 microseconds
        //  interRelaTime   指的是计算自相关函数的时间间隔
        //
        //

        if(!hasCalcuFreq)
            freqDomain();
        if(!hasCalcuLambda)                 //  计算带限之后的自相关函数
            afterBetaPower();

        const double interFreq = freqBand / (freqSamNum - 1);     //  频率间隔范围

        const double interRelaTime = relaTotalTime/relaTimeSamNum;

        const double startTime = -(relaTotalTime / 2);

        const unsigned long rBetaStartNum = (freqSamNum-1)/2 - rBeta / interFreq;
        const unsigned long rBetaEndNum = (freqSamNum-1)/2 + rBeta / interFreq;

        relaSample.resize(relaTimeSamNum);
        relaWaveForm.resize(relaTimeSamNum);
        absRelaWaveForm.resize(relaTimeSamNum);
        normalAbsRelaWaveForm.resize(relaTimeSamNum);

        for(unsigned long j=0;j<relaTimeSamNum;j++)   {

            relaSample[j] = startTime + interRelaTime * j;

            for(unsigned long i=rBetaStartNum;i<=rBetaEndNum;i++)   {

                //  求e(2*PI*f)
                complex<double> Wexp(cos(2*PI*freqSample[i]*relaSample[j]),sin(2*PI*freqSample[i]*relaSample[j]));

                //  求和操作
                //  这里的数据总是溢出

                relaWaveForm[j] += freqWaveForm[i] * Wexp * interFreq;

            }

            absRelaWaveForm[j] = abs(relaWaveForm[j]) / lambda;
        }

        //  归一化函数


        const double maxAbsRelaWaveForm = findMax(absRelaWaveForm);

        for(unsigned long j=0;j<relaTimeSamNum;j++) {

            normalAbsRelaWaveForm[j] = absRelaWaveForm[j] / maxAbsRelaWaveForm;

        }

        hasCalcuRela = true;
    }
    */

    void bpsk::autocorrelation() {

        //  relatotalTime       指的是自相关函数计算的时间范围，这里以 2 microseconds
        //  interRelaTime   指的是计算自相关函数的时间间隔
        //
        //

        if(!hasCalcuFreq)
            freqDomain();
        if(!hasCalcuLambda)                 //  计算带限之后的自相关函数
            afterBetaPower();

        emit sendProgress(0.2);

        const double interFreq = freqBand / (freqSamNum - 1);     //  频率间隔范围

        const double interRelaTime = relaTotalTime/relaTimeSamNum;

        const double startTime = -(relaTotalTime / 2);

        //const unsigned long rBetaStartNum = (freqSamNum-1)/2 - rBeta / interFreq;
        //const unsigned long rBetaEndNum = (freqSamNum-1)/2 + rBeta / interFreq;

        const unsigned long rBetaStartNum = 0;
        const unsigned long rBetaEndNum = freqSamNum - 1;

        double real;
        double imag;

        relaSample.resize(relaTimeSamNum);
        relaWaveForm.resize(relaTimeSamNum);
        absRelaWaveForm.resize(relaTimeSamNum);
        normalAbsRelaWaveForm.resize(relaTimeSamNum);


        for(unsigned long j=0;j<relaTimeSamNum;j++)   {

            relaSample[j] = startTime + interRelaTime * j;

            for(unsigned long i=rBetaStartNum;i<=rBetaEndNum;i++)   {

                //  求e(2*PI*f)
                //complex<double> Wexp(cos(2*PI*freqSample[i]*relaSample[j]),sin(2*PI*freqSample[i]*relaSample[j]));
                real += cos(2*PI*freqSample[i]*relaSample[j])*freqWaveForm[i];
                imag += sin(2*PI*freqSample[i]*relaSample[j])*freqWaveForm[i];

                //  求和操作
                //  这里的数据总是溢出

                //relaWaveForm[j] += freqWaveForm[i] * Wexp * interFreq;

            }

            relaWaveForm[j] = complex<double>(real * interFreq,imag * interFreq);
            real = imag = 0.0;

            absRelaWaveForm[j] = abs(relaWaveForm[j]) / lambda;

            sendProgressValue(j,relaTimeSamNum,0.2);
        }

        // 归一化函数
        const double maxAbsRelaWaveForm = findMax(absRelaWaveForm);

        for(unsigned long j=0;j<relaTimeSamNum;j++) {

            normalAbsRelaWaveForm[j] = absRelaWaveForm[j] / maxAbsRelaWaveForm;

        }

        hasCalcuRela = true;
    }
    /*  功能： 计算带限之后的剩余功率 lambda 带限 Beta r
     *  前提： 计算功率谱
     *
     *
     */

    void bpsk::afterBetaPower() {

        if(!hasCalcuFreq)
            freqDomain();

        const double interFreq = freqBand / (freqSamNum - 1);           //  频率间隔范围

        //const unsigned long rBetaStartNum = (freqSamNum-1)/2 - rBeta / interFreq;
        //const unsigned long rBetaEndNum = (freqSamNum-1)/2 + rBeta / interFreq;

        const unsigned long rBetaStartNum = 0;
        const unsigned long rBetaEndNum = freqSamNum - 1;

        for (unsigned long i = rBetaStartNum ; i < rBetaEndNum ; i++){
            lambda += freqWaveForm[i] * interFreq;                      //剩余功率
        }

        hasCalcuLambda = true;
    }
    /*  功能： 计算均方根带宽 RMS
     *  前提：计算功率谱，计算带限功率 lambda
     *
     *
     */

    void bpsk::calcuRMSBeta() {

        if(!hasCalcuLambda)
            afterBetaPower();

        const double interFreq = freqBand / (freqSamNum - 1);     //  频率间隔范围

        //const unsigned long rBetaStartNum = (freqSamNum-1)/2 - rBeta / interFreq;
        //const unsigned long rBetaEndNum = (freqSamNum-1)/2 + rBeta / interFreq;

        const unsigned long rBetaStartNum = 0;
        const unsigned long rBetaEndNum = freqSamNum - 1;

        double betarms;

        for (unsigned long i = rBetaStartNum ; i < rBetaEndNum ; i++){

              betarms += pow(freqSample[i],2.0) * freqWaveForm[i] / lambda * interFreq;
        }

        rmsBeta = sqrt(betarms);

        hasCalcuRMS = true;

    }

    /*  计算有效矩形带宽
     *  前提： freqWaveForm
     *
     *
     *
     */
    void bpsk::calcuRectBeta() {

        if(!hasCalcuFreq)
            freqDomain();
        if(!hasCalcuLambda)
            afterBetaPower();

        rectBeta = lambda / findMax(freqWaveForm);
    }

}
