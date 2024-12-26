
#include <vector>
#include "class_split.h"

class SplitDifference {
		
	public:
		SplitDifference();
		~SplitDifference() {}
		
		std::vector<std::vector<int>> additional_obs;
		std::vector<std::vector<int>> removed_obs;
		
		void computeSplitDifference(Split* split_upd, Split* split_prev);
};
