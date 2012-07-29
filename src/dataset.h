#ifndef _DATASET_H_
#define _DATASET_H_

#include <string>
#include <vector>

using namespace std;

class link_set {
public:
	int *links;
	int length;

	link_set() {
		links = NULL;
		length = 0;
	}

	link_set(int len) {
		length = len;
		links = new int[length];
	}
};

class rating {
public:
	int item;
	int star;
	int timestamp;

	rating() {}

	rating(int i, int s, int t) {
		item = i;
		star = s;
		timestamp = t;
	}
};

class rating_set {
public:
	rating *ratings;
	int length;
	double avg;
	rating_set() {
		length = 0;
		avg = 0;
		ratings = NULL;
	}
	rating_set(int len) {
		length = len;
		avg = 0;
		ratings = new rating[length];
	}
};

class dataset {
public:
	int n_user;
	int n_item;
	int n_v;
	bool has_time;
	string del;

	rating_set **user_rs;
	link_set **user_ls;

	dataset() {
		n_user = 0;
		n_item = 0;
		n_v = 0;
		user_rs = NULL;
		user_ls = NULL;
	}

	dataset(int nu, int ni, int nv, bool has_time, string del) {
		n_user = nu;
		n_item = ni;
		n_v = nv;
		user_rs = new rating_set*[n_user];
		user_ls = new link_set*[n_user];
		for (int i = 0; i < n_user; i++) {
			user_rs[i] = NULL;
			user_ls[i] = NULL;
		}
		this->has_time = has_time;
		this->del = del;
	}

	int read_ratings(string train_file, vector<int> *users);
	int read_links(string link_file);
};

#endif

