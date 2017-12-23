#ifndef BASESIGNAL_H_
#define BASESIGNAL_H_

#include <iostream>
#include <Qvector>
#include <QObject>


namespace sp {

    class baseSignal:public QObject {

        Q_OBJECT


    public:
        baseSignal(QVector<int> _a={1,-1,1,-1},QObject *parent = 0):
            a(_a),QObject(parent) {

        }

        const QVector<int> a;
        const int length = a.size();

    protected:

        void sendProgressValue(const unsigned long &num,const unsigned long &max,double start = 0.0);

    signals:    //  这里发送的是进度条   value   信号
        void sendProgress(double value);

    };

}

#endif
