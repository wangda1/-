#include "baseSignal.h"

namespace sp {


//  发射信号 进度条的当前值

void baseSignal::sendProgressValue(const unsigned long &num,const unsigned long &max,double start) {

    //double temp = (num / max) * (1.0 -start);

    emit sendProgress((double(num) / max) * (1.0 -start) + start);

}

}
