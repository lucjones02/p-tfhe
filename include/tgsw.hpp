#ifndef TGSW_HPP
#define TGSW_HPP

#include <vector>

#include <common.hpp>

class TLWESample;

class TGSWSample{
public:
    TGSWSample(const Bitmap& s, s_int sk_num);
    void encrypt(int mu);
    int decrypt();
    
    TLWESample operator[](int i) const;
    TLWESample& operator[](int i);
    TLWESample operator*(TLWESample c);

private:
    std::vector<TLWESample> c;
    bool is_encrypted;
    s_int secret_key_num;

};

#endif // !TGSW_HPP
