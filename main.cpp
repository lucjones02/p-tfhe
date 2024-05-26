#include <iostream>
#include <iomanip>
#include <cmath>

#include <common.hpp>
#include <generator.hpp>
#include <domain_expansion.hpp>
#include <tlwe.hpp>
#include <tgsw.hpp>

void erase_terminal(){
    std::cout << "\33[2K\r" << std::flush;
}

int main(){
    DomainExpansion sas;
    // t32 mu = 0b0000'0000'1100'0000'0000'1111'0000'0000;
    // TLWESample c = sas.encrypt(mu);
    // c = c + c;
    // std::cout << std::bitset<32>(abs((int)(mu + mu) - (int)(sas.decrypt(c)))) << std::endl;
    // std::cout << abs((int)(mu + mu) - (int)(sas.decrypt(c)))/2 << std::endl;
    t32 mu_1 = convert_to_t32(0.125);
    // t32 mu_2 = 300'000;
    TLWESample c_1 = sas.encrypt(mu_1);
    // TLWESample c_2 = sas.encrypt(mu_2);
    // std::cout << sas.decrypt(c_1) << "  " << sas.decrypt(c_2) << std::endl;
    // TLWESample c = c_1 + c_1;
    // double d = convert_to_double(sas.decrypt(c));
    // for(int i=2; i<=17; i++){
    //     std::cout <<"precision: " <<i << "   "<< std::setprecision(i) << d << std::endl;
    // }
    // t32 mu = sas.decrypt(c);
    // std::cout << std::bitset<32>(mu_1 * 2 + mu_2 * 5) << " " << std::bitset<32>(mu) << std::endl;
    // std::cout << std::setprecision(17) << (convert_to_double(mu_1) * 2 + convert_to_double(mu_2)* 5) << "  " << convert_to_double(mu) << std::endl;

    c_1.gadget_decomposition();
    c_1.mult_by_gadget_matrix();
    std::cout << std::setprecision(17) << convert_to_double(sas.decrypt(c_1)) << std::endl;

    // std::cout << std::endl;
    // int m = 4;
    // double res = 0;
    // for(int i=0; i<10'00; i++){
        // if(i%10 == 0){
        //     erase_terminal();
        //     std::cout << "progress " << (i/10) << "%" << std::flush;
        // }
    // TGSWSample C = sas.encrypt(m);
    //
    // TLWESample c = C * c_1;
    // res += convert_to_double(sas.decrypt(c));

    // }

    // std::cout << (res / 10'000.) << std::endl;
    // std::cout << res << std::endl;

}
