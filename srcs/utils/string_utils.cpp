
#include "string_utils.hpp"

std::string removeExtraSpaces(std::string &str) {
    std::string result;
    bool spaceFound = false;

    for (int i = 0; i < str.size(); i++) {
        if (isspace(str[i])) {
            if (!spaceFound) {
                result += ' ';
                spaceFound = true;
            }
        } else {
            result += str[i];
            spaceFound = false;
        }
    }
    return result;
}

std::vector<std::string> split(std::string str, char separator)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, separator)) {
        result.push_back(token);
    }
    return result;
}

bool isDigits(const std::string &str)
{
    return str.find_first_not_of("0123456789") == std::string::npos;
}