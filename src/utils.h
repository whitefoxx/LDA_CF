#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <utility>

using namespace std;

class model;
class model_tp;

class utils {
public:
    // parse command line arguments
    static int parse_args(int argc, char ** argv, model * pmodel);
	static int parse_args(int argc, char ** argv, model_tp * pmodel);
	static bool pro_cmp(pair<int, double> a, pair<int, double> b);
};

#endif

