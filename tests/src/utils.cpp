#include <utils.hpp>

#include <iostream>

void erase_terminal()
{
    std::cout << "\33[2K\r" << std::flush;
}

void update_progress(const std::string& func_name, int i){
    erase_terminal();
    std:: cout << func_name << "--"<< "progress: " << i << "%" << std::flush;
}
