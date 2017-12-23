/****   boc信号时域波形已测试，波形较密集，应改为 柱状图
 ****   频谱波形已测试通过，与材料上波形不符
 ****
 ****
 ****
 ****
 ****
 ****
 ****
 */
#ifndef BOC_H_
#define BOC_H_

#include "baseSignal.h"
#include <complex>
#include "toolfunc.h"

using std::complex;

namespace sp    {

    class boc:public baseSignal {

        Q_OBJECT

        public:

            const unsigned alpha;                   //
            const unsigned beta;

            QVector<double> timeSample;              //时间采样点
            QVector<double> freqSample;              //频率采样点
            QVector<double> relaSample;                //自相关函数 时间采样点

            QVector<double> timeWaveForm;                        //时域波形采样点
            QVector<double> freqWaveForm;                        //频域波形采样点
            QVector<double> freqDBWaveForm;                     //  分贝频域采样点
            QVector<complex<double>> relaWaveForm;               //自相关函数波形采样点
            QVector<double> absRelaWaveForm;                     //自相关函数波形采样点绝对值
            QVector<double> normalAbsRelaWaveForm;               //归一化自相关函数
            double lambda;                                      //  带限之后的剩余功率
            double rmsBeta;                                     //  均方根带宽
            double rectBeta;   //  有效矩形带宽 Beta Rectangle


            explicit boc(QVector<int> _a,unsigned _alpha,unsigned _beta):
                    baseSignal(_a),alpha(_alpha),beta(_beta) {

            }

            explicit boc(unsigned _alpha,unsigned _beta,
                const int _timeSamNumTb = 20,
                const unsigned long _freqSamNum = 10001,
                const double _freSampBand = 30E+6,
                const double _relaTotalTime = 2E-6,
                const unsigned long _relaTimeSamNum = 100,
                const double _rBeta = 12E+6):
                alpha(_alpha),beta(_beta),sampNumTb(_timeSamNumTb),freqSamNum(_freqSamNum),freSampBand(_freSampBand),
                    relaTotalTime(_relaTotalTime),relaTimeSamNum(_relaTimeSamNum),rBeta(_rBeta){

            }
            ~boc();
                                                    //bpsk 构造函数
            void freqDomain();                      //功率谱计算
            void timeDomain();                      //时域波形计算
            void autocorrelation();                 //计算自相关函数
            void afterBetaPower();                 //  Beta r带限之后的剩余功率
            void calcuRMSBeta();                         //  Beta r 带限之后的均方根带宽
            void calcuRectBeta();

    private:

            /*  信号的基本信息设置
             *
             */
            const double fs = alpha * 1.023E+6;         //亚载波频率
            const double fc = beta  * 1.023E+6;         //扩频码频率
            const double Tb = 1 / fc;                   //  码元持续时间
            const double Ts = 1 / (2*fs);               //  亚载波持续时间
            const int N = 2*alpha/beta;                 //  一个码元内亚载波半周期数目 n

            /*  采样时间与频率设置
             */
            const double freSampBand = 30E+6;                        //频谱采样带宽
            const double relaTotalTime = 2E-6;                        //  自相关函数采样时间 -1 microsecond -- 1 microsecond


            /*  这里采用对亚载波进行采样，20个码元时间，当N = 4时，每个Ts，
             *  亚载波半周期内则进行  20/4 = 5    个采样点采样
             */
            const int sampNumTb = 20;                               //  一个码元内采样点
            const double rBeta = 12E+6;                             //  自相关函数带限带宽为 24MHz
    public:
            const unsigned long timeSamNum = sampNumTb * length;                     //时域中采样点数   20 是指每个码元时间内有20个采样点
            const unsigned long freqSamNum = 10001;                           //频域中采样
            const unsigned long relaTimeSamNum = 100;                         //  自相关函数取样点数

    private:

            bool hasCalcuFreq = false;
            bool hasCalcuRela = false;
            bool hasCalcuLambda = false;
            bool hasCalcuRMS = false;

            //      连接进度条信号与MainWindow w SLOT连接
            //void Initconnect();


        };


}



#endif
