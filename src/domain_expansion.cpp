#include <cmath>
#include <iostream>
#include <common.hpp>
#include <domain_expansion.hpp>
#include <generator.hpp>

DomainExpansion::DomainExpansion(){
    Generator* gen = Generator::get_instance();
    auto [secret_key_, secret_key_num_] = gen->generate_secret_key();
    secret_key = secret_key_;
    secret_key_number = secret_key_num_;
}


TLWESample DomainExpansion::encrypt(t32 mu)
{
    TLWESample c(secret_key, secret_key_number);
    c.encrypt(mu);
    return c;
}

TLWESample DomainExpansion::message_space_encrypt(int m)
{
    return encrypt((t32) (m%p)<<(31-message_space_n));
}

TLWESample DomainExpansion::message_space_encrypt(t32 mu)
{
    if((mu & (1 << (31-message_space_n))) == 0){
        return encrypt(mu & (UINT_MAX << (31-message_space_n)));
    }
    else{
        return encrypt((mu & (UINT_MAX << (31-message_space_n))) + (1 << (31-message_space_n)));
    }
}

TGSWSample DomainExpansion::encrypt(int m)
{
    TGSWSample c(secret_key, secret_key_number);
    c.encrypt(m);
    return c;
}


t32 DomainExpansion::decrypt(TLWESample c)
{
    t32 sa = 0;

    for(int i=0; i<k_dim; i++){
        sa += secret_key[i]*c[i];
    }
    return c[k_dim] - sa;
}

int DomainExpansion::message_space_decrypt(TLWESample c)
{
    t32 mu_bar = decrypt(c);
    return convert_to_t32((round(p * convert_to_double(mu_bar))) / p);
}

int DomainExpansion::decrypt(TGSWSample C)
{
    return (int) this->decrypt(C[(k_dim+1)*l_gd-1]);
}
