#include "boc.h"
#include "constants.h"
#include "toolfunc.h"
#include <QVector>
#include <complex>
#include <cmath>

using std::sin;
using std::cos;
using std::tan;
using std::pow;
using std::log10;
using std::abs;     //这个用于 complex的绝对值求值，重载在 std 里，而非 stdlib

namespace sp {

    boc::~boc() {



    }

    /*  双极性亚载波产生
     *  开始为高电平
     */
    int subCarrier(int num) {
       if(num % 2) {
           return -1;
       }
       else return 1;
    }

    void boc::timeDomain() {


        timeSample.resize(timeSamNum);
        timeWaveForm.resize(timeSamNum);
        const double interTime = Ts / N;            //  亚载波半周期的采样间隔
        const int sampNumTs = sampNumTb/N;          //  每个亚载波半周期    Ts  的采样点数
        double samTime = 0.0;

        /*  i   为第i个码元
         *  j   为第j个半载波周期
         *  k   为一个半载波周期的第  k   个点
         */
        for(int i=0;i<length;i++) {
            for(int j=0;j<N;j++) {
                for(int k=0;k<sampNumTs;k++) {

                      timeSample[i*sampNumTb+j*sampNumTs+k] = samTime + (i*sampNumTb+j*sampNumTs+k)*interTime;

                      timeWaveForm[i*sampNumTb+j*sampNumTs+k] = subCarrier(i*N+j) * a[i];

                }
            }
        }
    }


    void boc::freqDomain() {

        if(hasCalcuFreq)
            return;

        freqSample.resize(freqSamNum);
        freqWaveForm.resize(freqSamNum);
        freqDBWaveForm.resize(freqSamNum);

        double freStart = -(freSampBand/2);
        double interFreq = freSampBand / freqSamNum;

        if(!(N % 2)) {
            for(unsigned long i=0;i<freqSamNum;i++) {
                freqSample[i] = freStart + i*interFreq;

                freqWaveForm[i] =  fc * pow( (tan(PI*freqSample[i]/(2*fs)) * sin(PI*freqSample[i]/fc)) /
                                           (PI * freqSample[i]) , 2.0);

                freqDBWaveForm[i] = 10*log10(freqWaveForm[i]);

            }
        }
        else {
            for(unsigned long i=0;i<freqSamNum;i++) {
                freqSample[i] = freStart + i*interFreq;


                freqWaveForm[i] =  fc * pow( (tan(PI*freqSample[i]/(2*fs)) * cos(PI*freqSample[i]/fc)) /
                                           (PI * freqSample[i]) , 2.0);

                freqDBWaveForm[i] = 10*log10(freqWaveForm[i]);

            }

        }

        hasCalcuFreq = true;

    }

    /*  求自相关函数，参考公式见材料
     *  主要算法：求傅里叶反变换
     *
     *
     *
     *
     */
    /*
    void boc::autocorrelation() {

        //  自相关函数带限带宽的范围为 rBeta  -12--12MHz
        //  自相干函数波形取样点  relaTimeSamNum = 100
        //  自相关函数波形时间范围： relaTotalTime   -1E-6 -- 1E-6: -1 microsecond -- 1 microsecond
        //  自相关函数时间采样间隔： interRelaTime

        if(!hasCalcuFreq)
            freqDomain();

        const double interRelaTime = relaTotalTime / relaTimeSamNum;
        const double startTime = -(relaTotalTime / 2);

        double freStart = -(freSampBand/2);
        double interFreq = freSampBand / freqSamNum;

        const unsigned long rBetaStartNum = (-rBeta - freStart) / interFreq;      //  计算带宽起始点位置
        const unsigned long rBetaEndNum = freqSamNum - 1 - rBetaStartNum;         //  计算带宽终点位置

        relaSample.resize(relaTimeSamNum);
        relaWaveForm.resize(relaTimeSamNum);
        absRelaWaveForm.resize(relaTimeSamNum);
        normalAbsRelaWaveForm.resize(relaTimeSamNum);



        for(unsigned long j=0;j<relaTimeSamNum;j++)   {

            relaSample[j] = startTime + interRelaTime * j;

            for(unsigned long i=rBetaStartNum ;i<=rBetaEndNum;i++)   {

                //  求e(2*PI*f*t)
                complex<double> Wexp(cos(2*PI*freqSample[i]*relaSample[j]),sin(2*PI*freqSample[i]*relaSample[j]));

                //  求和操作
                //  这里的数据总是溢出

                relaWaveForm[j] += freqWaveForm[i] * Wexp * interFreq;

            }

            absRelaWaveForm[j] = abs(relaWaveForm[j]);
        }
    }
    */

        void boc::autocorrelation() {

            //  自相关函数带限带宽的范围为 rBeta  -12--12MHz
            //  自相干函数波形取样点  relaTimeSamNum = 100
            //  自相关函数波形时间范围： relaTotalTime   -1E-6 -- 1E-6: -1 microsecond -- 1 microsecond
            //  自相关函数时间采样间隔： interRelaTime

            if(!hasCalcuFreq)
                freqDomain();
            if(!hasCalcuLambda)
                afterBetaPower();

            emit sendProgress(0.2);

            const double interRelaTime = relaTotalTime / relaTimeSamNum;
            const double startTime = -(relaTotalTime / 2);

            double freStart = -(freSampBand/2);
            double interFreq = freSampBand / freqSamNum;

            const unsigned long rBetaStartNum = (-rBeta - freStart) / interFreq;      //  计算带宽起始点位置
            const unsigned long rBetaEndNum = freqSamNum - 1 - rBetaStartNum;         //  计算带宽终点位置

            relaSample.resize(relaTimeSamNum);
            relaWaveForm.resize(relaTimeSamNum);
            absRelaWaveForm.resize(relaTimeSamNum);
            normalAbsRelaWaveForm.resize(relaTimeSamNum);

            double real = 0.0;
            double imag = 0.0;



            for(unsigned long j=0;j<relaTimeSamNum;j++)   {

                relaSample[j] = startTime + interRelaTime * j;

                for(unsigned long i=rBetaStartNum ;i<=rBetaEndNum;i++)   {

                    //  求e(2*PI*f*t)
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
                //qDebug()<<"Now the Progress Value is"<<

            }

        //  归一化函数
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

    void boc::afterBetaPower() {

        if(!hasCalcuFreq)
            freqDomain();

        double freStart = -(freSampBand/2);
        double interFreq = freSampBand / freqSamNum;

        const unsigned long rBetaStartNum = (-rBeta - freStart) / interFreq;      //  计算带宽起始点位置
        const unsigned long rBetaEndNum = freqSamNum - 1 - rBetaStartNum;         //  计算带宽终点位置

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
    void boc::calcuRMSBeta() {

        if(!hasCalcuRMS)
            afterBetaPower();

        const double freStart = -(freSampBand/2);
        const double interFreq = freSampBand / freqSamNum;

        const unsigned long rBetaStartNum = (-rBeta - freStart) / interFreq;      //  计算带宽起始点位置
        const unsigned long rBetaEndNum = freqSamNum - 1 - rBetaStartNum;         //  计算带宽终点位置


        double betarms;

        for (unsigned long i = rBetaStartNum ; i < rBetaEndNum ; i++){

              betarms += pow(freqSample[i],2.0) * freqWaveForm[i] / lambda * interFreq;
        }

        rmsBeta = sqrt(betarms);                                                    //均方根

        hasCalcuRMS = true;

    }


    void boc::calcuRectBeta() {

        if(!hasCalcuFreq)
            freqDomain();
        if(!hasCalcuLambda)
            afterBetaPower();

        rectBeta = lambda / findMax(freqWaveForm);
    }


}


