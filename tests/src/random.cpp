#include <iostream>
#include <string>

#include <random.hpp>
#include <utils.hpp>
#include <logger.hpp>

#include <generator.hpp>

void test_torus_sampling()
{
    Generator* gen = Generator::get_instance();
    Logger* logger = Logger::get_instance("test_all_correctness");

    int res = 0;
    for(int i=0; i<1000*TESTS; i++){
        res += (int)gen->sample_error();
    }

    if(abs(res) <= 500*TESTS){
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

void test_torus_zeroes()
{
    Generator* gen = Generator::get_instance();
    Logger* logger = Logger::get_instance("test_all_correctness");

    int res = 0;
    for(int i=0; i<1000*TESTS; i++){
        if((int)gen->sample_error() == 0){
            res++;
        }
    }

    if(res <= 4*TESTS){
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
