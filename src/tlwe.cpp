#include <cmath>
#include <iostream>

#include <tlwe.hpp>
#include <common.hpp>
#include <generator.hpp>

// generates a TLWE sample encrypting 0
TLWESample::TLWESample(const Bitmap& s, s_int sk_num):
    is_encrypted(false),
    secret_key_num(sk_num)
{
    Generator* gen = Generator::get_instance();
    t32 sa = 0; // s.a running sum of secret times by samples for the last entry of the sample
    for(int i=0; i<k_dim; i++){
        t32 ai = gen->sample_torus(); // current sample at index i
        a.push_back(ai);
        sa += s[i]*ai; // add value of the secret at index i time the sample at index i (s[i] is 0 or 1)
    }
    t32 e = gen->sample_error(); // error of the sample
    a.push_back(sa + e); // index k_dim | math equivalent: index (k+1)
}

TLWESample::TLWESample(TLWESample* s):
    is_encrypted(s->is_encrypted),
    secret_key_num(s->secret_key_num)
{
    for(int i=0; i<=k_dim; i++){
        a.push_back(0);
    }
}

void TLWESample::encrypt(t32 mu){
    if(is_encrypted){
        throw std::runtime_error("TLWESample already encrypted");
    }
    a[k_dim] += mu;
    is_encrypted = true;
}

void TLWESample::gadget_decomposition()
{
    std::vector<int> u;
    for(const auto& ai : a){
        int64_t closest_mult = round(((int32_t) ai) * (round(std::pow(B_gd, l_gd)) / round(std::pow(2, 32))));
        std::vector<int> temp_vec{};
        for(int p=1; p<= l_gd; p++){
            int64_t temp = closest_mult % (int64_t)std::pow(B_gd, p);
            if(temp >= (int64_t)std::pow(B_gd, p)/2){
                temp -= (int64_t)std::pow(B_gd, p);
            }
            else if(temp < -(int64_t)std::pow(B_gd, p)/2){
                temp += (int64_t)std::pow(B_gd, p);
            }
            closest_mult -= temp;
            temp = temp / (int)std::pow(B_gd, p-1);
            temp_vec.push_back(temp);
        }
        for(int i=temp_vec.size()-1; i>=0; i--){
            u.push_back(temp_vec[i]);
        }
    }

    gadget_decomp = u;
}

void TLWESample::mult_by_gadget_matrix()
{
    assert(gadget_decomp.size()==((k_dim+1)*l_gd));

    for(int i=0; i <= k_dim; i++){
        uint32_t temp = 0;
        for(int j=0; j < l_gd; j++){
            temp +=  (uint32_t) round(std::pow(B_gd, l_gd-1-j)) * gadget_decomp[i*l_gd + j];
        }
        a[i] = temp;
    }
}

TLWESample TLWESample::operator+(TLWESample const& m)
{
    TLWESample sum(this);
    assert(secret_key_num == m.secret_key_num);

    for(int i=0; i<=k_dim; i++){
        sum[i] = a[i] + m.a[i];
    }

    return sum;
}

TLWESample TLWESample::operator*(int k){
    TLWESample res(this);

    for(int i=0; i<=k_dim; i++){
        res[i] = a[i] * k;
    }

    return res;
}

TLWESample TLWESample::operator*(TGSWSample C)
{
    TLWESample res(this);
    this->gadget_decomposition();

    for(int i=0; i < (k_dim + 1)*l_gd; i++){
        TLWESample sample = C[i];
        for(int j=0; j <= k_dim; j++){
            res[j] += gadget_decomp[j] * sample[j];
        }
    }

    return res;
}

t32 TLWESample::operator[](int i) const
{
    return a[i];
}

t32& TLWESample::operator[](int i)
{
    return a[i];
}

ModdedTLWESample::ModdedTLWESample(const Bitmap& s, s_int sk_num):
    is_encrypted(false),
    secret_key_num(sk_num)
{
    Generator* gen = Generator::get_instance();
    t32 sa = 0; // s.a running sum of secret times by samples for the last entry of the sample
    uint8_t mu_overflow = 0; // start counting the overflows for mu
    // for(int i=0; i<10; i++){
    for(int i=0; i<k_dim; i++){
        t32 ai = gen->sample_torus(); // current sample at index i
        a.push_back(ai);
        overflows.push_back(0);
        if(overflowed(sa, (s[i]*ai))){
            mu_overflow++;
        }
        sa += s[i]*ai; // add value of the secret at index i time the sample at index i (s[i] is 0 or 1)
    }
    t32 e = gen->sample_error(); // error of the sample
    if((int)e > 0){
        if(overflowed(sa, e)){
            mu_overflow++;
        }
    }
    // std::cout << "error " << (int)e << std::endl;
    a.push_back(sa + e); // index k_dim | math equivalent: index (k+1)
    overflows.push_back(mu_overflow);
}

ModdedTLWESample::ModdedTLWESample(ModdedTLWESample* s):
    is_encrypted(s->is_encrypted),
    secret_key_num(s->secret_key_num)
{
    for(int i=0; i<=k_dim; i++){
        a.push_back(0);
        overflows.push_back(0);
    }
}

void ModdedTLWESample::encrypt(t32 mu, int p_){
    p = p_;
    // std::cout << std::bitset<64>(p*(uint64_t)mu) << std::endl;
    uint32_t overs = (p*(uint64_t)mu) >> 32; 
    // std::cout << "overs " << (int)overs << std::endl;
    overflows[k_dim] += overs;
    if(is_encrypted){
        throw std::runtime_error("TLWESample already encrypted");
    }
    if(overflowed(a[k_dim], (uint32_t)p*mu)){
        overflows[k_dim]++;
    }
    // std::cout << "encrypted " << (int) (p*mu) << "  "<< std::bitset<32>(p*mu) << std::endl;
    a[k_dim] += p*mu;
    is_encrypted = true;
}
t32 ModdedTLWESample::operator[](int i) const
{
    return a[i];
}

t32& ModdedTLWESample::operator[](int i)
{
    return a[i];
}

uint32_t ModdedTLWESample::get_overflows(int i)
{
    return overflows[i];
}

int ModdedTLWESample::get_p()
{
    return p;
}

ModdedTLWESample ModdedTLWESample::operator+(ModdedTLWESample const& m)
{
    ModdedTLWESample sum(this);
    assert(secret_key_num == m.secret_key_num);

    for(int i=0; i<=k_dim; i++){
        sum[i] = a[i] + m.a[i];
        sum.overflows[i] = overflows[i] + m.overflows[i];
        if(overflowed(a[i], m.a[i])){
            sum.overflows[i]++;
        }
    }

    return sum;
}
