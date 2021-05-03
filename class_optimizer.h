#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include <armadillo>

class Optimizer {
	
  public:
    Optimizer(arma::mat* data);
    
    arma::mat* data;
	void optimize();
    
};


class ExhaustiveSearch: public Optimizer {
	public:
		using Optimizer::Optimizer;
		using Optimizer::optimize;
		virtual void optimize();
};



#endif 
