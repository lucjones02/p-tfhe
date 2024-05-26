#include <logger.hpp>

#include <fstream>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <iomanip>


Logger* Logger::instance = nullptr;

Logger::Logger(const char* func_name){
    std::filesystem::path file_path = this->find_results_folder() / this->make_name_unique(func_name);
    file.open(file_path.string());
}

Logger* Logger::get_instance(const std::string& str){
    if(instance == nullptr){
        instance = new Logger(str.c_str());
    }
    return instance;
}


Logger::~Logger(){
    file.close();
}

void Logger::destroy()
{
    this->~Logger();
}

std::filesystem::path Logger::find_results_folder()
{
    std::filesystem::path current_path = std::filesystem::current_path();
    
    for(const auto& dir : std::filesystem::directory_iterator(current_path))
    {
        if(dir.is_directory() && dir.path().stem() == "results_dump"){
            return dir;
        }
    }

    throw std::runtime_error("results directory does not exist");
}

std::string Logger::make_name_unique(const char* func_name)
{
    std::stringstream ss;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    ss << func_name << "__" << std::put_time(&tm, "%m_%d__%H_%M_%S") << ".results";
    return ss.str();
}

void Logger::log(const std::string& str)
{
    file << str << std::endl;
}

