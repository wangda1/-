#ifndef TOOLFUNC_H
#define TOOLFUNC_H

#include <QVector>

/*  模板函数文件
 *  模板类模板函数因为在编译时不能分配空间，故不能进行单独编译
 *  必须把实现与声明写在一个头文件里！！！
 *
 *
 */

namespace sp {

    template<typename Type>
    Type findMax(QVector<Type> &V) {
        Type Max = V[0];
        for(int i=0;i<V.size();i++) {
            if(V[i] > Max) {
                Max = V[i];
            }
        }
        return Max;
    }

    template<typename Type>
    Type findMin(QVector<Type> &V) {
        Type Min = V[0];
        for(int i=0;i<V.size();i++) {
            if(V[i] < Min) {
                Min = V[i];
            }
        }
        return Min;
    }
}

#endif // TOOLFUNC_H
