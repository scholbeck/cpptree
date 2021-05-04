#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <armadillo>

class Optimizer {
	
  public:
    Optimizer(Data* data);
    
    Data* data;
	void optimize();
    
};

class ExhaustiveSearch: public Optimizer {
	public:
		using Optimizer::Optimizer;
		using Optimizer::optimize;
		virtual void optimize();
};

class RandomSearch: public Optimizer {
	public:
		using Optimizer::Optimizer;
		using Optimizer::optimize;
		virtual void optimize();
};

#endif 
