#include "class_formula.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>

Formula::Formula() {
}

void Formula::setString(std::string str) {
    this->formula_string = str;
}

void Formula::processString() {
    std::vector<std::string> tokens;
    std::istringstream isstream{this->formula_string};
    std::string buffer;
    while (getline(isstream, buffer, '+')) {
        tokens.push_back(buffer);
    }
    for (int i = 0; i < tokens.size(); ++i) {
        tokens[i].erase(std::remove(tokens[i].begin(), tokens[i].end(), 'x'), tokens[i].end());
        this->features.push_back(std::stoi(tokens[i]));
    }
}

std::vector<int> Formula::getFeatures() {
    return this->features;
}