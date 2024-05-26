#include <iostream>

#include <generator.hpp>
#include <domain_expansion.hpp>
#include <tlwe.hpp>

#include <additions.hpp>
#include <utils.hpp>
#include <logger.hpp>

void test_add_correctness_tlwe()
{
    Generator* gen = Generator::get_instance();
    DomainExpansion sas;
    Logger* logger = Logger::get_instance("test_all_correctness");

    t32 mu_1 = gen->sample_torus();
    t32 mu_2 = gen->sample_torus();

    int error = 0;
    for(int i=0; i<100; i++){
        t32 expected_result = 0;
        t32 res = 0;
        for(int j=0; j<TESTS; j++){
            TLWESample c_1 = sas.encrypt(mu_1);
            TLWESample c_2 = sas.encrypt(mu_2);

            TLWESample c = c_1 + c_2;

            expected_result += mu_1 + mu_2;
            res += sas.decrypt(c);
        }
        error += ((int) res - (int) expected_result);
        update_progress(__func__, i);
    }
    erase_terminal();


    if(abs(error) <= 50*TESTS){
        std::string str = std::string(__func__) + ": success";
        std::cout << str << std::endl;
        logger->log(str);
    }
    else{
        std::string str = std::string(__func__) + ": fail";
        std::cout << str << std::endl;
        logger->log(str);
    }
}
