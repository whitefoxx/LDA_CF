#ifndef _CORPUS_H_
#define _CORPUS_H_

#include <string>
#include <map>

using namespace std;

class document {
public:
	int *words;
	int length;
	document() {
		length = 0;
		words = NULL;
	}
	document(int len) {
		length = len;
		words = new int[length];
	}
};

class corpus {
public:
	int n_doc;
	int n_word;
	map<string, int> word2id;
	map<int, string> id2word;

	document **docs;

	corpus() {
		n_doc = 0;
		n_word = 0;
		docs = NULL;
	}

	int read_data(string train_file);
};

#endif

