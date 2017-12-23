#include "toolfunc.h"

/*  求最值模板
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
