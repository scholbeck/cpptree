
#include <stdlib.h>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
	
    std::vector<std::string> exec_msgs;
    exec_msgs.push_back(std::string("../code/tree --filename ../data/bh.data --target 13 --sep , --task regr --algorithm exhaustive --objective sse --model mean --minsize 10 --children 2 --maxdepth 10 --print false"));
    int n_exec_msgs = exec_msgs.size();
    for (int i = 0; i < n_exec_msgs; i++) {
        system(exec_msgs[i].c_str());
    }
    return EXIT_SUCCESS;
}