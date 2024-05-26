#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <filesystem>

class Logger{
public:
    ~Logger();

    static Logger* get_instance(const std::string& str);

    void destroy();

    std::filesystem::path find_results_folder();
    std::string make_name_unique(const char* func_name);

    void log(const std::string& str);

    Logger(Logger& other) = delete;
    void operator=(const Logger& other) = delete;

protected:

    static Logger* instance;

private:
    Logger(const char* func_name);
    std::ofstream file;
};

#endif // !LOGGER_HPP
