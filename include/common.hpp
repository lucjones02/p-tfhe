#ifndef COMMON_HPP
#define COMMON_HPP

#include <vector>

typedef uint32_t t32;
typedef unsigned int s_int;

constexpr int k_dim = 512;
constexpr int B_gd = 2;
constexpr int l_gd = 32;
constexpr int message_space_n = 20;
constexpr int p = 1 << (message_space_n-1);

class Bitmap{
    private:
        uint32_t bits[(k_dim >> 5) + 1];

    public:
        uint64_t operator [](int i) const;

        void set(int i, uint32_t val);
};

double convert_to_double(t32 mu);
t32 convert_to_t32(double d);


#endif // !COMMON_HPP
