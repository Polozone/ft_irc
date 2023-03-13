
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../utils/string_utils.cpp"

void modeCommand(std::string &userInput)
{
    std::string inputExtraSpaceRemoved = removeExtraSpaces(userInput);
    std::vector<std::string> output = split(inputExtraSpaceRemoved, ' ');
    std::vector<std::string>::iterator it;
    it = output.begin();
    if (*it == "")
        ++it;
    for (; it != output.end(); ++it) {
        std::cout << ";;;" << *it << ";;;" << std::endl;
    }
}