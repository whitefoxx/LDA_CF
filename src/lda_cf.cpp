#include "model.h"
#include "model_tp.h"
#include <cstdio>

using namespace std;

void usage_cf();
void usage_tp();

int main(int argc, char ** argv)
{
	string arg = argv[1];
	if (arg == "-CF") {
		model lda;

		if (lda.init(argc, argv)) {
			usage_cf();
			return 1;
		}

		lda.estimate();
	}
	else if (arg == "-TP") {
		model_tp lda_tp;

		if (lda_tp.init(argc, argv)) {
			usage_tp();
			return 1;
		}

		lda_tp.estimate();
	}
	else {
		printf("You must specify which model you would like to use: -CF for collaborative\
filtering, -TP for topic model.\n");
		usage_cf();
		usage_tp();
	}

	return 0;
}

void usage_cf() {
	printf("Command line usage for collaborative filtering:\n");
	printf("\tlda -CF -train_file <string> -alpha <double> -beta <double> -n_topic <int> \
-n_iter <int> -n_user <int> -n_item <int> -n_v <int> -test_file <string>\n");
}

void usage_tp() {
	printf("Command line usage for topic model:\n");
	printf("\tlda -TP -train_file <string> -alpha <double> -beta <double> -eta <double> -n_topic <int> \
-savestep <int> -tpNwd <int> -docNtp <int>\n");
}