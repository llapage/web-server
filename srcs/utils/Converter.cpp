#include "../../includes/utils/Converter.hpp"
#include <stdexcept>
#include <stdlib.h>

int Converter::toInt(const std::string &str)
{
    char *end;
    unsigned long number = strtol(str.c_str(), &end, 10);
    if (*end != '\0') // If conversion stopped before the end of the string
    {
        throw std::invalid_argument(str);
    }
    return number;
}

unsigned int Converter::toUInt(const std::string &str)
{
    char *end;
    unsigned long number = strtoul(str.c_str(), &end, 10);
    if (*end != '\0') // If conversion stopped before the end of the string
    {
        throw std::invalid_argument(str);
    }
    return number;
}

std::string Converter::toString(double value) { return to_string(value); }

std::string Converter::toString(int value) { return to_string(value); }

std::string Converter::toString(unsigned long value)
{
    return to_string(value);
}

std::string Converter::toString(long value) { return to_string(value); }

std::string Converter::toString(float value) { return to_string(value); }
