#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <common.hpp>

class Generator{
private:
    s_int num_secret_keys;
    std::vector<Bitmap> all_secret_keys;

protected:
    Generator();
    static Generator* instance;

public:
    // Singleton deletions
    Generator(Generator& other) = delete;
    void operator=(const Generator& other) = delete;

    static Generator* get_instance();


    std::pair<Bitmap, s_int> generate_secret_key();
    Bitmap get_secret_key(s_int i);

    t32 sample_torus() const;
    t32 sample_error() const;
};


#endif // !GENERATOR_HPP
