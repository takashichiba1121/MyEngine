 #include "Easing.h"
#include<math.h>

float Easing::easeOutBounce(float x)
{
    const float n1 = 7.5625f;
    const float d1 = 2.75f;

    if (x < 1 / d1) {
        return n1 * x * x;
    }
    else if (x < 2 / d1) {
        return n1 * (x -= 1.5f / d1) * x + 0.75f;
    }
    else if (x < 2.5 / d1) {
        return n1 * (x -= 2.25f / d1) * x + 0.9375f;
    }
    else {
        return n1 * (x -= 2.625f / d1) * x + 0.984375f;
    }
}
float Easing::easeOutSine(float x){
return static_cast<float>(sin((x * 3.141592f) / 2));
}
