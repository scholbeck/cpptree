#include "class_formula.h"
#include <string>



Formula::Formula() {
}

void Formula::setString(std::string str) {
    this->formula_string = str;
}

void Formula::processString() {

    for (int i = 0; i < this->formula_string.length(); i++) {
        this->features.push_back((int) this->formula_string[i]);
    }
}


