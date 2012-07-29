#include "model.h"
#include "utils.h"
#include "config.h"
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>

int model::init(int argc, char ** argv) {
	if(utils::parse_args(argc, argv, this))
		return PASER_ERROR;

	p = new double[n_topic];

	// read training data and test data
	ptr_train_data = new dataset(n_user, n_item, n_v, has_time, del);
	if (ptr_train_data->read_ratings(train_file, &train_user)) {
		printf("Fail to read training data!\n");
		return READ_DATA_ERROR;
	}
	ptr_test_data = new dataset(n_user, n_item, n_v, has_time, del);
	if (ptr_test_data->read_ratings(test_file, &test_user)) {
		printf("Fail to read test data!\n");
		return READ_DATA_ERROR;
	}

	nit = new int*[n_item];
	for (int i = 0; i < n_item; i++) {
		nit[i] = new int[n_topic];
		for (int j = 0; j < n_topic; j++) {
			nit[i][j] = 0;
		}
	}

	nut = new int*[n_user];
	for (int m = 0; m < n_user; m++) {
		nut[m] = new int[n_topic];
		for (int j = 0; j < n_topic; j++) {
			nut[m][j] = 0;
		}
	}

	nt = new int[n_topic];
	for (int k = 0; k < n_topic; k++) {
		nt[k] = 0;
	}

	nu = new int[n_user];
	for (int m = 0; m < n_user; m++) {
		nu[m] = 0;
	}

	nitv = new int**[n_item];
	for (int i = 0; i < n_item; i++) {
		nitv[i] = new int*[n_topic];
		for (int k = 0; k < n_topic; k++) {
			nitv[i][k] = new int[n_v];
			for (int v = 0; v < n_v; v++)
				nitv[i][k][v] = 0;
		}
	}

	srand(time(0));
	z = new int*[n_user];
	for (vector<int>::iterator it = train_user.begin(); it != train_user.end(); it++) {
		int m = *it;
		int N = ptr_train_data->user_rs[m]->length;
		z[m] = new int[N];
		// initialize for z
		int topic;
		for (int n = 0; n < N; n++) {
			do { topic = (int)(((double)rand() / RAND_MAX) * n_topic); }
			while (topic >= n_topic);
			z[m][n] = topic;
			int i = ptr_train_data->user_rs[m]->ratings[n].item;
			int v = ptr_train_data->user_rs[m]->ratings[n].star - 1;
			nit[i][topic] += 1;
			nitv[i][topic][v] += 1;
			nut[m][topic] += 1;
			nt[topic] += 1;
			nu[m] += 1;
		} 
	}

	theta = new double*[n_user];
	for (int m = 0; m < n_user; m++) {
		theta[m] = new double[n_topic];
	}

	phi = new double**[n_topic];
	for (int k = 0; k < n_topic; k++) {
		phi[k] = new double*[n_item];
		for (int i = 0; i < n_item; i++) {
			phi[k][i] = new double[n_v];
		}
	}    

	return 0;
}

void model::estimate()
{
	double rmse, prob, sprob, sstar, star, err, theta_s, phi_s;
	int topic, count;
	for (int iter = 0; iter < n_iter; iter++) {
		for (vector<int>::iterator it = train_user.begin(); it != train_user.end(); it++) {
			int m = *it;
			for (int n = 0; n < ptr_train_data->user_rs[m]->length; n++) {
				topic = sampling(m, n);
				z[m][n] = topic;
			}
		}
		compute_theta();
		compute_phi();

		rmse = 0.0;
		count = 0;
		for (vector<int>::iterator it = test_user.begin(); it != test_user.end(); it++) {
			int m = *it;
			theta_s = 0;
			for (int k = 0; k < n_topic; k++) {
				theta_s += theta[m][k];
			}
			for (int n = 0; n < ptr_test_data->user_rs[m]->length; n++) {
				sprob = sstar = phi_s = 0;
				int i = ptr_test_data->user_rs[m]->ratings[n].item;				
				for (int v = 0; v < n_v; v++) {
					prob = 0;					
					for (int k = 0; k < n_topic; k++) {
						phi_s = 0;
						for (int r = 0; r < n_v; r++)
							phi_s += phi[k][i][r];
						prob += (theta[m][k] / theta_s) * (phi[k][i][v] / phi_s);
					}
					/*sprob += prob;*/
					sstar += prob * (v + 1);
				}
				star = sstar/* / sprob*/;

				err = (double)ptr_test_data->user_rs[m]->ratings[n].star - star;
				rmse += err * err;
				count ++;
			}
		}
		rmse = sqrt(rmse / count);
		printf("RMSE of iterator %3d:	%f, count: %d\n", iter, rmse, count);
	}
}

int model::sampling(int m, int n) {
	// remove z_i from the count variables
	int topic = z[m][n];
	int i = ptr_train_data->user_rs[m]->ratings[n].item;
	int v = ptr_train_data->user_rs[m]->ratings[n].star - 1;
	nitv[i][topic][v] --;
	nit[i][topic] --;
	nut[m][topic] --;
	nu[m] -= 1;

	double Vbeta = 0;
	double Kalpha = n_topic * alpha;    

	/**** 以下是一个根据p[k]抽样的过程 ****/
	for (int r = 0; r < n_v; r++)
		Vbeta += beta[r];
	for (int k = 0; k < n_topic; k++) {
		p[k] = (nitv[i][k][v] + beta[v]) / (nit[i][k] + Vbeta) *
			(nut[m][k] + alpha) / (nu[m] + Kalpha);
	}
	// cumulate multinomial parameters
	for (int k = 1; k < n_topic; k++) {
		p[k] += p[k - 1];
	}
	// scaled sample because of unnormalized p[]
	double u = ((double)rand() / RAND_MAX) * p[n_topic - 1];
	for (topic = 0; topic < n_topic; topic++) {
		if (p[topic] >= u) {
			break;
		}
	}

	// add newly estimated z_i to count variables
	nitv[i][topic][v] ++;
	nit[i][topic] ++;
	nut[m][topic] ++;
	nu[m] ++;    

	return topic;
}

void model::compute_theta() {
	double Kalpha = n_topic * alpha;
	for (int m = 0; m < n_user; m++) {
		for (int k = 0; k < n_topic; k++) {
			theta[m][k] = (nut[m][k] + alpha) / (nu[m] + Kalpha);
		}
	}
}

void model::compute_phi() {
	double Vbeta = 0;
	for (int v = 0; v < n_v; v++)
		Vbeta += beta[v];
	for (int k = 0; k < n_topic; k++) {
		for (int i = 0; i < n_item; i++) {
			for (int v = 0; v < n_v; v++) {
				phi[k][i][v] = (nitv[i][k][v] + beta[v]) / (nit[i][k] + Vbeta);
			}
		}
	}
}
