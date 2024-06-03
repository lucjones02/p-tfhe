#include <iostream>
#include <random>

#include <common.hpp>
#include <generator.hpp>

std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());
Generator* Generator::instance = nullptr;

Generator::Generator():num_secret_keys(0){}

Generator* Generator::get_instance(){
    if(instance == nullptr){
        instance = new Generator();
    }
    return instance;
}

std::pair<Bitmap, s_int> Generator::generate_secret_key()
{
    std::uniform_int_distribution<uint32_t> u_distribution(0,1<<31);
    Bitmap bitmap;
    for(int i=0; i<= (k_dim >> 5); i++){
        bitmap.set(i, u_distribution(generator));
    }
    all_secret_keys.push_back(bitmap);
    return std::make_pair(bitmap, num_secret_keys++);
}

Bitmap Generator::get_secret_key(s_int i){
    if(i >= all_secret_keys.size()){
        throw std::runtime_error("accessing secret key that does not exist");
    }
    return all_secret_keys[i];
}

t32 Generator::sample_torus() const
{
    std::uniform_int_distribution<uint32_t> u_distribution(0,UINT32_MAX);
    return u_distribution(generator);
}

t32 Generator::sample_error() const
{
    std::normal_distribution<float> noise(0, 128.f);
    float e = (noise(generator));
    if(e>=0){
        return round(e);
    }
    else{
        return -((int32_t)round(abs(e)));
    }
}
