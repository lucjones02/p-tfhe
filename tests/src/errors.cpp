#include <tlwe.hpp>
#include <domain_expansion.hpp>
#include <errors.hpp>
#include <logger.hpp>
#include <common.hpp>
#include <utils.hpp>

void get_error_precision_single_addition_tlwe()
{
    Generator* gen = Generator::get_instance();
    DomainExpansion sas;
    Logger* logger = Logger::get_instance("results");

    t32 mu_1 = gen->sample_torus();
    t32 mu_2 = gen->sample_torus();

    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("                             " + std::string(__func__));
    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("--------------------------------------------------------------------------------------------");

    double final_precision = 0;
    int prec[32] = {0};
    for(int i=0; i<100; i++){
        t32 expected_result = mu_1 + mu_2;
        t32 res = 0;
        int total_precision =0;
        for(int j=0; j<TESTS; j++){
            TLWESample c_1 = sas.encrypt(mu_1);
            TLWESample c_2 = sas.encrypt(mu_2);

            TLWESample c = c_1 + c_2;

            res = sas.decrypt(c);
            int error = abs((int) res - (int) expected_result);
            int precision = 0;
            while(error){
                precision += 1;
                error = error >> 1;
            }
            prec[precision] += 1;
            total_precision += precision;
        }
        update_progress(__func__, i);
        final_precision += total_precision / (double) TESTS;
    }
    erase_terminal();

    std::cout << __func__ << ": done" << std::endl;
    for(int i=0; i<32; i++){
        if(prec[i]){
            std::string str = "error bit size " + std::to_string(i) + ": " + std::to_string(prec[i]);
            logger->log(str);
        }
    }
    std::string str = "average error bit size: " + std::to_string(final_precision/100);
    logger->log(str);
}
void get_error_precision_double_addition_tlwe()
{
    Generator* gen = Generator::get_instance();
    DomainExpansion sas;
    Logger* logger = Logger::get_instance("results");

    t32 mu_1 = gen->sample_torus();
    t32 mu_2 = gen->sample_torus();
    t32 mu_3 = gen->sample_torus();

    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("                             " + std::string(__func__));
    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("--------------------------------------------------------------------------------------------");

    double final_precision = 0;
    int prec[32] = {0};
    for(int i=0; i<100; i++){
        t32 expected_result = mu_1 + mu_2 + mu_3;
        t32 res = 0;
        int total_precision =0;
        for(int j=0; j<TESTS; j++){
            TLWESample c_1 = sas.encrypt(mu_1);
            TLWESample c_2 = sas.encrypt(mu_2);
            TLWESample c_3 = sas.encrypt(mu_3);

            TLWESample c = (c_1 + c_2) + c_3;

            res = sas.decrypt(c);
            int error = abs((int) res - (int) expected_result);
            int precision = 0;
            while(error){
                precision += 1;
                error = error >> 1;
            }
            prec[precision] += 1;
            total_precision += precision;
        }
        update_progress(__func__, i);
        final_precision += total_precision / (double) TESTS;
    }
    erase_terminal();

    std::cout << __func__ << ": done" << std::endl;
    for(int i=0; i<32; i++){
        if(prec[i]){
            std::string str = "error bit size " + std::to_string(i) + ": " + std::to_string(prec[i]);
            logger->log(str);
        }
    }
    std::string str = "average error bit size: " + std::to_string(final_precision/100);
    logger->log(str);
}

void get_error_precision_additions_tlwe()
{
    Generator* gen = Generator::get_instance();
    DomainExpansion sas;
    Logger* logger = Logger::get_instance("results");

    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("                             " + std::string(__func__));
    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("--------------------------------------------------------------------------------------------");
    
    std::vector<int> num_additions{10, 20, 30, 40, 50};
    for(const auto& n:num_additions){
        logger->log("--n: " + std::to_string(n));

        std::vector<t32> mu_vec;
        t32 expected_result = 0;
        for(int i=0; i<n; i++){
            t32 mu = gen->sample_torus();
            expected_result += mu;
            mu_vec.push_back(mu);
        }

        double final_precision = 0;
        int prec[32] = {0};
        for(int i=0; i<100; i++){
            t32 res = 0;
            int total_precision =0;
            for(int j=0; j<TESTS; j++){
                std::vector<TLWESample> c_vec;
                TLWESample c = sas.encrypt(mu_vec[0]);
                for(int i=1; i<n; i++){
                    c = c + sas.encrypt(mu_vec[i]);
                }

                res = sas.decrypt(c);
                int error = abs((int) res - (int) expected_result);
                int precision = 0;
                while(error){
                    precision += 1;
                    error = error >> 1;
                }
                prec[precision] += 1;
                total_precision += precision;
            }
            update_progress(std::string(__func__) + " (n=" + std::to_string(n) + ")", i);
            final_precision += total_precision / (double) TESTS;
        }
        erase_terminal();

        std::cout << __func__ << "(n=" << n << "): done" << std::endl;
        for(int i=0; i<32; i++){
            if(prec[i]){
                std::string str = "\t\terror bit size " + std::to_string(i) + ": " + std::to_string(prec[i]);
                logger->log(str);
            }
        }
        std::string str = "\t\taverage error bit size: " + std::to_string(final_precision/100);
        logger->log(str);
        }
}

void get_error_precision_linear_comb_tlwe()
{
    Generator* gen = Generator::get_instance();
    DomainExpansion sas;
    Logger* logger = Logger::get_instance("results");

    t32 mu_1 = gen->sample_torus();
    t32 mu_2 = gen->sample_torus();
    int k_1 = (int) gen->sample_error();
    int k_2 = (int) gen->sample_error();

    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("                             " + std::string(__func__));
    logger->log("--------------------------------------------------------------------------------------------");
    logger->log("--------------------------------------------------------------------------------------------");

    double final_precision = 0;
    int prec[32] = {0};
    for(int i=0; i<100; i++){
        t32 expected_result = k_1 * mu_1 + k_2 * mu_2;
        t32 res = 0;
        int total_precision =0;
        for(int j=0; j<TESTS; j++){
            TLWESample c_1 = sas.encrypt(mu_1);
            TLWESample c_2 = sas.encrypt(mu_2);

            TLWESample c = c_1 * k_1 + c_2 * k_2;

            res = sas.decrypt(c);
            int error = abs((int) res - (int) expected_result);
            int precision = 0;
            while(error){
                precision += 1;
                error = error >> 1;
            }
            prec[precision] += 1;
            total_precision += precision;
        }
        update_progress(__func__, i);
        final_precision += total_precision / (double) TESTS;
    }
    erase_terminal();

    std::cout << __func__ << ": done" << std::endl;
    for(int i=0; i<32; i++){
        if(prec[i]){
            std::string str = "error bit size " + std::to_string(i) + ": " + std::to_string(prec[i]);
            logger->log(str);
        }
    }
    std::string str = "average error bit size: " + std::to_string(final_precision/100);
    logger->log(str);
}
