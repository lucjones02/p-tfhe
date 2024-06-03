#include <decode.hpp>
#include <generator.hpp>
#include <logger.hpp>
#include <utils.hpp>
#include <common.hpp>
#include <domain_expansion.hpp>


void test_decode()
{
    Generator* gen = Generator::get_instance();
    Logger* logger = Logger::get_instance("test_all_correctness");

    std::vector<int> spacing_values{523, 1021, 2027, 3733, 5231, 6043, 7817, 9613, 13681, 19387};
    // std::vector<int> spacing_values{5231, 6043, 7817, 9613, 13681, 19387};

    int failed = false;
    for(const auto& p : spacing_values){
        if(failed){break;}
        int res = 0;
        for(int i=0; i<100*TESTS; i++){
            t32 mu = gen->sample_torus();
            DomainExpansion sas;
            ModdedTLWESample c = sas.encrypt(mu, p);
            t32 mu_bar = sas.decrypt(c);
            res += (mu_bar == mu) ? 0 : 1;
            // if(mu_bar != mu){
                // std::cout << std::bitset<32>(mu) << std::endl;
                // std::cout << std::bitset<32>(mu_bar) << std::endl;
                // std::cout << std::endl;
                // failed=true;
                // break;
            // }

            if(i%TESTS == 0){
                update_progress(std::string(__func__) + " (p=" + std::to_string(p) + ")", i/TESTS);
            }
        }
        erase_terminal();

        std::string str = std::string(__func__) + " (p="  + std::to_string(p) + "): " + std::to_string(res) + " decoding errors";
        std::cout << str << std::endl;
        logger->log(str);
    }

}

void test_decode_single_add()
{
    Generator* gen = Generator::get_instance();
    Logger* logger = Logger::get_instance("test_all_correctness");

    std::vector<int> spacing_values{3733, 5231, 6043, 7817, 9613, 13681, 19387};
    // std::vector<int> spacing_values{5231, 6043, 7817, 9613, 13681, 19387};

    int failed = false;
    for(const auto& p : spacing_values){
        if(failed){break;}
        int res = 0;
        for(int i=0; i<100*TESTS; i++){
            t32 mu = gen->sample_torus();
            t32 mu_2 = gen->sample_torus();

            DomainExpansion sas;
            ModdedTLWESample c = sas.encrypt(mu, p);
            ModdedTLWESample c_2 = sas.encrypt(mu_2, p);
            t32 mu_bar = sas.decrypt(c + c_2);
            res += (mu_bar == (mu + mu_2)) ? 0 : 1;

            if(i%TESTS == 0){
                update_progress(std::string(__func__) + " (p=" + std::to_string(p) + ")", i/TESTS);
            }
        }
        erase_terminal();

        std::string str = std::string(__func__) + " (p="  + std::to_string(p) + "): " + std::to_string(res) + " decoding errors";
        std::cout << str << std::endl;
        logger->log(str);
    }

}
