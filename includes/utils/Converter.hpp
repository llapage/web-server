#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <sstream>
#include <string>

template <typename T> std::string to_string(T value)
{
    std::ostringstream ostr;
    ostr << value;
    return ostr.str();
}

class Converter
{
private:
    Converter();

public:
    static int toInt(const std::string &str);
    static unsigned int toUInt(const std::string &str);
    static std::string toString(double value);
    static std::string toString(int value);
    static std::string toString(float value);
    static std::string toString(unsigned long value);
    static std::string toString(long value);
};

#endif // CONVERTER_HPP
