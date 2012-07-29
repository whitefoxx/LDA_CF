#ifndef _MODEL_H_
#define _MODEL_H_

#include "dataset.h"

class model {
public:
	string train_file;
	string test_file;
	string dir;
	string del;
	bool has_time;

	int n_user;					// number of user, index begin at 0
	int n_item;					// number of item, index begin at 0
	int n_topic;				// number of topic, index begin at 0
	int n_v;					// number of different star
	int n_iter;					// max iterators
	double alpha;				// LDA hyper parameters
	double *beta;				// LDA hyper parameters
	double eta;
	dataset *ptr_train_data;	// pointer to training data
	dataset *ptr_test_data;		// pointer to test data
	vector<int> train_user;		// users in training data
	vector<int> test_user;		// users in test data

	double *p;	// probability for sampling
	int **z;	// topic assignments for items, size M x (user's rating size x n_v)
	int **nit;	// nit[i][j]: number of instances of item i assigned to topic j, size (n_item x n_v) x n_topic
	int **nut;	// nut[i][j]: number of items in user_i's ratings assigned to topic j, size n_user x n_topic
	int *nt;	// nt[i]: number of items assigned to topic i, size n_topic
	int *nu;	// nu[i]: number of ratings of user i, size n_user
	double **theta; // theta: user-topic distributions, size n_user x n_topic
	double ***phi;	 // phi: topic-item distributions, size n_topic x n_item x n_v
	int ***nitv;

	model() {
		del = " ";
		has_time = false;
		beta = NULL;
		ptr_train_data = NULL;
		ptr_test_data = NULL;
		p = NULL;
		z = NULL;
		nit = NULL;
		nut = NULL;
		nt = NULL;
		nu = NULL;
		theta = NULL;
		phi = NULL;
	}

	int init(int argc, char ** argv);
	void estimate();
	int sampling(int m, int n);
	void compute_theta();
	void compute_phi();
};

#endif

