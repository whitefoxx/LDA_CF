#include "model_tp.h"
#include "utils.h"
#include "config.h"
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

int model_tp::init(int argc, char ** argv) {
	if(utils::parse_args(argc, argv, this))
		return PASER_ERROR;

	p = new double[n_topic];

	// read training data and test data
	ptr_train_data = new corpus;
	if (ptr_train_data->read_data(train_file)) {
		printf("Fail to read training data!\n");
		return READ_DATA_ERROR;
	}
	printf("n_doc:  %d\n", ptr_train_data->n_doc);
	printf("n_word: %d\n", ptr_train_data->n_word);

	n_word = ptr_train_data->n_word;
	nwt = new int*[n_word];
	for (int i = 0; i < n_word; i++) {
		nwt[i] = new int[n_topic];
		for (int j = 0; j < n_topic; j++) {
			nwt[i][j] = 0;
		}
	}

	n_doc = ptr_train_data->n_doc;
	ndt = new int*[n_doc];
	for (int m = 0; m < n_doc; m++) {
		ndt[m] = new int[n_topic];
		for (int j = 0; j < n_topic; j++) {
			ndt[m][j] = 0;
		}
	}

	snwt = new int[n_topic];
	for (int k = 0; k < n_topic; k++) {
		snwt[k] = 0;
	}

	snwd = new int[n_doc];
	for (int m = 0; m < n_doc; m++) {
		snwd[m] = 0;
	}

	srand(time(0));
	z = new int*[n_doc];
	for (int m = 0; m < n_doc; m++) {
		int N = ptr_train_data->docs[m]->length;
		z[m] = new int[N];
		// initialize for z
		for (int n = 0; n < N; n++) {
			int topic = (int)(((double)rand() / RAND_MAX) * n_topic);
			z[m][n] = topic;
			// number of instances of item i assigned to topic j
			int w = ptr_train_data->docs[m]->words[n];
			nwt[w][topic] += 1;
			// number of items in user i's ratings assigned to topic j
			ndt[m][topic] += 1;
			// total number of items assigned to topic j
			snwt[topic] += 1;
		} 
		// total number of words in document i
		snwd[m] = N;      
	}

	theta = new double*[n_doc];
	for (int m = 0; m < n_doc; m++) {
		theta[m] = new double[n_topic];
	}

	phi = new double*[n_topic];
	for (int k = 0; k < n_topic; k++) {
		phi[k] = new double[n_word];
	}    

	printf("Initial done.\n");
	return 0;
}

void model_tp::estimate()
{
	for (int iter = 1; iter <= n_iter; iter++) {
		for (int m = 0; m < n_doc; m++) {
			for (int n = 0; n < ptr_train_data->docs[m]->length; n++) {
				int topic = sampling(m, n);
				z[m][n] = topic;
			}
		}

		if (savestep > 0) {
			if (iter % savestep == 0) {
				printf("Saving the model at iteration %d ...\n", iter);
				compute_theta();
				compute_phi();
				save_model(iter);
			}
		}
	}
}

int model_tp::sampling(int m, int n) {
	// remove z_i from the count variables
	int topic = z[m][n];
	int w = ptr_train_data->docs[m]->words[n];
	nwt[w][topic] -= 1;
	ndt[m][topic] -= 1;
	snwt[topic] -= 1;
	snwd[m] -= 1;

	double Vbeta = n_word * beta;
	double Kalpha = n_topic * alpha;    

	/**** 以下是一个根据p[k]抽样的过程 ****/
	for (int k = 0; k < n_topic; k++) {
		p[k] = (nwt[w][k] + beta) / (snwt[k] + Vbeta) *
			(ndt[m][k] + alpha) / (snwd[m] + Kalpha);
	}
	// cumulate multinomial parameters
	for (int k = 1; k < n_topic; k++) {
		p[k] += p[k - 1];
	}
	// scaled sample because of unnormalized p[]
	//srand(time(0));
	double u = ((double)rand() / RAND_MAX) * p[n_topic - 1];
	for (topic = 0; topic < n_topic; topic++) {
		if (p[topic] > u) {
			break;
		}
	}

	// add newly estimated z_i to count variables
	nwt[w][topic] += 1;
	ndt[m][topic] += 1;
	snwt[topic] += 1;
	snwd[m] += 1;    

	return topic;
}

void model_tp::compute_theta() {
	for (int m = 0; m < n_doc; m++) {
		for (int k = 0; k < n_topic; k++) {
			theta[m][k] = (ndt[m][k] + alpha) / (snwd[m] + n_topic * alpha);
		}
	}
}

void model_tp::compute_phi() {
	for (int k = 0; k < n_topic; k++) {
		for (int w = 0; w < n_word; w++) {
			phi[k][w] = (nwt[w][k] + beta) / (snwt[k] + n_word * beta);
		}
	}
}

void model_tp::save_model(int iter)
{
	save_topicNword(iter);
	save_docNtopic(iter);
}

void model_tp::save_topicNword(int iter)
{
	char fn[100];
	sprintf(fn, "%s/topicNword_%d-%d.txt", dir.c_str(), iter - savestep, iter);
	FILE *fp = fopen(fn, "w");
	vector< pair<int, double> > p_word;
	for (int k = 0; k < n_topic; k++) {		
		for (int w = 0; w < n_word; w++)
			p_word.push_back(make_pair(w, phi[k][w]));
		sort(p_word.begin(), p_word.end(), utils::pro_cmp);
		fprintf(fp, "Topic %d\n", k);
		for (int n = 0; n < tpNwd; n++) {
			fprintf(fp, "\t%s\t\t%f\n", ptr_train_data->id2word[p_word[n].first].c_str(), p_word[n].second);
		}
		p_word.clear();
	}
	fclose(fp);
}

void model_tp::save_docNtopic(int iter)
{
	char fn[100];
	sprintf(fn, "%s/docNtopic_%d-%d.txt", dir.c_str(), iter - savestep, iter);
	FILE *fp = fopen(fn, "w");
	vector< pair<int, double> > p_topic;
	for (int m = 0; m < n_doc; m++) {
		for (int k = 0; k < n_topic; k++)
			p_topic.push_back(make_pair(k, theta[m][k]));
		sort(p_topic.begin(), p_topic.end(), utils::pro_cmp);
		fprintf(fp, "Document %d\n", m);
		for (int n = 0; n < docNtp; n++) {
			fprintf(fp, "\t%d\t\t%f\n", p_topic[n].first, p_topic[n].second);
		}
		p_topic.clear();
	}
	fclose(fp);
}