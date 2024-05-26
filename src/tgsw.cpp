#include <cmath>

#include <domain_expansion.hpp>
#include <generator.hpp>
#include <tgsw.hpp>
#include <common.hpp>


TGSWSample::TGSWSample(const Bitmap& s, s_int sk_num):
    is_encrypted(false),
    secret_key_num(sk_num)
{
    for(int i=0; i<(k_dim+1)*l_gd; i++){
        c.push_back(TLWESample(s, sk_num));
    }
}

void TGSWSample::encrypt(int mu)
{
    if(is_encrypted){
        throw std::runtime_error("TGSWSample already encrypted");
    }
    for(int batch=0; batch < k_dim+1; batch++){
        for(int l=0; l<l_gd; l++){
            c[batch*l_gd + l][batch] += (t32) (mu * convert_to_t32(std::pow(B_gd, -(l+1))));
        }
    }
    is_encrypted = true;
}

int TGSWSample::decrypt()
{
    DomainExpansion dec;
    return std::pow(B_gd, l_gd) * dec.decrypt(c[(k_dim+1)*l_gd-1]);
}

TLWESample TGSWSample::operator[](int i) const
{
    return c[i];
}


TLWESample& TGSWSample::operator[](int i)
{
    return c[i];
}

TLWESample TGSWSample::operator*(TLWESample c)
{
    return c * (*this);
}
