#pragma once
#include <exception>

class NoConfigException: public std::exception{
    const char* what() const noexcept override{
        return "config file is empty.";
    }
};
class NoConfigFileException: public std::exception{
    const char* what() const noexcept override{
        return "config file is missing.";
    }
};