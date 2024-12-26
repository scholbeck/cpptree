#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include <vector>

class Formula {
	
  public:
    Formula();
    ~Formula() {}
    
    std::string formula_string;
    std::vector<int> features;

    void setString(std::string);
    void processString();
    std::vector<int> getFeatures();
    
};
#endif 