
#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

std::string removeExtraSpaces(std::string &str);
std::vector<std::string> split(std::string str, char separator);
bool isDigits(const std::string &str);
std::string createStringWithParams(int indexToStart, std::vector<std::string> inputClient);