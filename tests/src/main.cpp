#include <additions.hpp>
#include <random.hpp>
#include <errors.hpp>
#include <decode.hpp>

#include <string>

int main(int argc, char *argv[]){
    std::string type;
    if(argc == 2){
        type = argv[1];
    }

    if((type.size() == 0) || (type == "correctness")){
        // test_torus_sampling();
        // test_torus_zeroes();
        // test_add_correctness_tlwe();
        // test_decode();
        test_decode_single_add();
    }
    else{
        get_error_precision_single_addition_tlwe();
        get_error_precision_double_addition_tlwe();
        get_error_precision_additions_tlwe();
        get_error_precision_linear_comb_tlwe();
    }
}
