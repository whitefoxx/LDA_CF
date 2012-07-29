#ifndef _MODEL_TP_H_
#define _MODEL_TP_H_

#include "corpus.h"

class model_tp {
public:
	string train_file;
	string test_file;
	string dir;

	int n_doc;					// number of user, index begin at 0
	int n_word;					// number of item, index begin at 0
	int n_topic;				// number of topic, index begin at 0
	int n_iter;					// max iterators
	int savestep;
	int tpNwd;
	int docNtp;
	double alpha, beta;			// LDA hyper parameters
	corpus *ptr_train_data;	// pointer to training data
	corpus *ptr_test_data;		// pointer to test data

	double *p;	// probability for sampling
	int **z;	// topic assignments for items, size M x (user's rating size x n_v)
	int **nwt;	// nit[i][j]: number of instances of item i assigned to topic j, size (n_item x n_v) x n_topic
	int **ndt;	// nut[i][j]: number of items in user_i's ratings assigned to topic j, size n_user x n_topic
	int *snwt;	// nt[i]: number of items assigned to topic i, size n_topic
	int *snwd;	// nu[i]: number of ratings of user i, size n_user
	double ** theta; // theta: user-topic distributions, size n_user x n_topic
	double ** phi;	 // phi: topic-item distributions, size n_topic x (n_item x n_v)


	model_tp() {
		n_doc = 0;
		n_word = 0;
		n_topic = 0;
		n_iter = 0;
		savestep = 0;
		tpNwd = 0;
		docNtp = 0;
		ptr_train_data = NULL;
		ptr_test_data = NULL;
		p = NULL;
		z = NULL;
		nwt = NULL;
		ndt = NULL;
		snwt = NULL;
		snwd = NULL;
		theta = NULL;
		phi = NULL;
	}

	int init(int argc, char ** argv);
	void estimate();
	int sampling(int m, int n);
	void compute_theta();
	void compute_phi();
	void save_model(int iter);
	void save_topicNword(int iter);
	void save_docNtopic(int iter);
};

#endif

