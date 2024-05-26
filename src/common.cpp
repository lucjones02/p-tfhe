#include <cmath>
#include <iostream>
#include <common.hpp>

uint64_t Bitmap::operator[](int i) const
{
    return (bits[(i>>5)] & (1 << i%32)) >> (i%32);
}

void Bitmap::set(int i, uint32_t val)
{
    bits[i] = val;
}

double convert_to_double(t32 mu)
{
    double res = 0;
    for(int i=0; i<32; i++){
        res += std::pow(2., -(i+1)) * ((mu & (1 << (31-i))) >> (31-i));
    }
    return res;
}

t32 convert_to_t32(double d)
{
    assert(d <= 1);
    return (t32) (d * std::pow(2, 32));
}
