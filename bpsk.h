/****  时域波形已测试---波形比较密集
 ****  频域波形已测试---波形还好
 ****
 ****
 ****
 ****
 ****
 ****
 ****
 */
#ifndef BPSK_H_
#define BPSK_H_

#include <cmath>
#include <complex>
#include <QtDebug>
#include <QObject>
#include "baseSignal.h"
#include "constants.h"
#include "toolfunc.h"

using std::complex;

namespace sp {

    class bpsk:public baseSignal {

        Q_OBJECT

    public:

        const double fb;                          //码速率  fb
        const double fc;                          //载频    fc

        QVector<double> timeSample;              //时间采样点
        QVector<double> freqSample;              //频率采样点
        QVector<double> relaSample;                //自相关函数 时间采样点

        QVector<double> timeWaveForm;                        //时域波形采样点
        QVector<double> freqWaveForm;                        //频域波形采样点
        QVector<double> freqDBWaveForm;                     //  频域 分贝采样点
        QVector<complex<double>> relaWaveForm;               //自相关函数波形采样点
        QVector<double> absRelaWaveForm;                     //自相关函数波形采样点绝对值
        QVector<double> normalAbsRelaWaveForm;               //归一化自相关函数
        double lambda;                                      //  带限之后的剩余功率   Lambda
        double rmsBeta;                                     //  均方根带宽   RMS
        double rectBeta;                                    //  有效矩形带宽 Beta Rectangle


        explicit bpsk(double _fb,double _fc,QVector<int> _a,QObject *parent=0):
                fb(_fb),fc(_fc),baseSignal(_a,parent) {

        }

        explicit bpsk(double _fb,double _fc,
             const int _timeSamNumTb = 20,
             const unsigned long _freqSamNum = 10001,
             const int _NFreqBand = 4,
             const double _relaTotalTime = 2E-6,
             const unsigned long _relaTimeSamNum = 100,
             const double _rBeta = 12E+6):
                fb(_fb),fc(_fc),timeSamNumTb(_timeSamNumTb),freqSamNum(_freqSamNum),NFreqBand(_NFreqBand),
                    relaTotalTime(_relaTotalTime),relaTimeSamNum(_relaTimeSamNum),rBeta(_rBeta){
        }

        ~bpsk();
                                                //bpsk 构造函数
        void freqDomain();                      //功率谱计算
        void timeDomain();                      //时域波形计算
        void autocorrelation();                 //计算自相关函数
        void afterBetaPower();                 //  Beta r带限之后的剩余功率
        void calcuRMSBeta();                   //  Beta r 带限之后的均方根带宽
        void calcuRectBeta();

    private:


        const double Tb = 1 / fb;                       //码元持续时间    Tb
        const double relaTotalTime = 2E-6;               //  自相关函数截取时间   单位：s

        const int timeSamNumTb = 20;                    //时域中采样点数   20 是指每个码元时间内有20个采样点

    public:

        const unsigned long timeSamNum = timeSamNumTb * length;
        const unsigned long freqSamNum = 10001;                           //频域中采样
        const unsigned long relaTimeSamNum = 100;                         //  自相关函数取样点数

    private:

        const int NFreqBand = 4;                                //  频域中     FB  的个数
        const double rBeta = 12E+6;                             //  自相关函数带限带宽为 24MHz
        const double freqBand = NFreqBand * fb;                 //  频域带宽4个 fb的频率范围

        bool hasCalcuFreq = false;
        bool hasCalcuRela = false;
        bool hasCalcuLambda = false;
        bool hasCalcuRMS = false;


    };



}



#endif
