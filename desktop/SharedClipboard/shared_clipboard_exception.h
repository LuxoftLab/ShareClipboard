#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdexcept>

class WrongPasswordError : public std::runtime_error
{
    const std::string errorstr = "You entered a wrong password";
public:
    WrongPasswordError() : std::runtime_error(errorstr){}

    const char * what() const throw()
    {
        return errorstr.c_str();
    }
};

#endif // EXCEPTION_H

