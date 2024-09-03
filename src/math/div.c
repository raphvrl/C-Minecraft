#include "div.h"

int div_floor(int a, int b)
{
    int result = a / b;
    if ((a < 0) != (b < 0) && (a % b != 0)) {
        result--;
    }
    return result;
}