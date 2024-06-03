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

ModdedTLWESample DomainExpansion::encrypt(t32 mu, int p)
{
    ModdedTLWESample c(secret_key, secret_key_number);
    c.encrypt(mu, p);
    return c;
}

TLWESample DomainExpansion::message_space_encrypt(int m)
{
    return encrypt((t32) (m%p_ms)<<(31-message_space_n));
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

t32 DomainExpansion::decrypt(ModdedTLWESample c)
{
    t32 sa = 0;
    uint32_t overflows = 0;
    uint32_t sa_overflow = 0;

#ifdef DEBUG
    uint64_t control = 0;
    std::cout << "-----------------------------------------------------------------------------------------------------" << std::endl;
#endif
    for(int i=0; i<k_dim; i++){
        if(addition_overflowed(sa, secret_key[i]*c[i])){
            sa_overflow++;
        }
#ifdef DEBUG
        control += secret_key[i]*c[i];
        std::cout << std::bitset<32>(control) << std::endl;
#endif
        sa += secret_key[i]*c[i];
        overflows += secret_key[i]*c.get_overflows(i);
    }

    uint32_t total_overflows = sa_overflow + overflows;
    if(addition_overflowed((t32)(c[k_dim]-sa), (uint32_t)sa)){
        total_overflows++;
    }
    uint32_t pmu_overflows = c.get_overflows(k_dim) - total_overflows;
    pmu_overflows = pmu_overflows % c.get_p();
    uint32_t quotient = ((uint64_t)0b1 << 32) / c.get_p();
    uint32_t modulus = ((uint64_t)0b1 << 32) % c.get_p();
    int leeway  = c.get_p() >> 1;

    uint32_t min_val = pmu_overflows==0 ? 0 : pmu_overflows * (quotient) * c.get_p() + (modulus*pmu_overflows / c.get_p()) * c.get_p() + c.get_p();
    uint32_t max_val;
    if(min_val < modulus){
        max_val = min_val + quotient * c.get_p();
    }
    else{
        max_val = min_val + (quotient-1) * c.get_p();
    }
    uint32_t corrected = c[k_dim] - sa - min_val;
    if(c[k_dim]-sa < min_val){
        if((int)(c[k_dim]-sa) < (int)(min_val - leeway)){
            return (min_val - (t32)c.get_p())*get_inverse_mod(c.get_p());
        }
        return min_val*get_inverse_mod(c.get_p());
    }
    uint32_t decode_index = corrected / c.get_p();
    uint32_t decode_index_mod = corrected % c.get_p();
#ifdef DEBUG
    std::cout << "sa overs  " << sa_overflow << std::endl;
    std::cout << "overflows " << overflows << std::endl;
    std::cout << "total overs  " << total_overflows << std::endl;
    std::cout << "pmu overflows  " << (int)pmu_overflows << std::endl;
    std::cout << "mod " << modulus << std::endl;
    std::cout << "min val " << (min_val) << std::endl;
    std::cout << "max val " << (int)max_val << "  " << std::bitset<32>(max_val) << std::endl;
    std::cout << "pmu " << (int)(c[k_dim] -sa) << "  "<< std::bitset<32>(c[k_dim]-sa) << std::endl;
    std::cout << "decode index " << decode_index << std::endl;
    std::cout << "decode_index_mod " << decode_index_mod << std::endl;
    std::cout << "leeway "<< leeway << std::endl;
#endif
    if(decode_index_mod > leeway){
#ifdef DEBUG
        std::cout << "decode_index_mod larger" << std::endl;
#endif
        decode_index++;
    }
#ifdef DEBUG
    std::cout << std::bitset<32>(c[k_dim]-sa) << std::endl;
    std::cout << "intermediate " << std::bitset<32>(decode_index*c.get_p()) << std::endl;
    std::cout << std::bitset<32>(min_val + decode_index * c.get_p())<< std::endl;
#endif // DEBUG
    return (min_val + decode_index*c.get_p())* get_inverse_mod(c.get_p());
}

int DomainExpansion::message_space_decrypt(TLWESample c)
{
    t32 mu_bar = decrypt(c);
    return convert_to_t32((round(p_ms * convert_to_double(mu_bar))) / p_ms);
}

int DomainExpansion::decrypt(TGSWSample C)
{
    return (int) this->decrypt(C[(k_dim+1)*l_gd-1]);
}
