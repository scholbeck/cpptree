#ifndef MODEL_H
#define MODEL_H

#include "class_model.h"
#include "class_data.h"


class Model {
  
  public:
    Model(Data* training_data);
    
    Data* training_data;
    
    std::vector<double> predict(Data* data);
};

class ModelConstant: public Model {
	public:
		using Model::Model;
		using Model::predict;
		virtual std::vector<double> predict(Data* data);
};

class ModelLinearRegression: public Model {
	public:
		using Model::predict;
};


#endif 
