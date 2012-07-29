#include "corpus.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include "config.h"

int corpus::read_data(string data_file)
{
	FILE *fp = fopen(data_file.c_str(), "r");
	if (fp == NULL)
		return OPEN_FILE_ERROR;

	const int MAX_LEN = 2 << 14;
	char line[MAX_LEN];
	char *tok;
	fgets(line, MAX_LEN, fp);
	n_doc = atoi(line);
	docs = new document*[n_doc];
	vector<int> tmp;
	int id;
	for (int i = 0; i < n_doc; i++) {
		fgets(line, MAX_LEN, fp);
		line[strlen(line)-1] = '\0';
		tok = strtok(line, " ");
		while (tok) {
			if (word2id.find(string(tok)) == word2id.end()) {
				id = word2id.size();
				word2id[string(tok)] = id;
				id2word[id] = string(tok);
			}
			tmp.push_back(word2id[string(tok)]);
			tok = strtok(NULL, " ");
		}
		document *doc = new document(tmp.size());
		docs[i] = doc;
		int n = 0;
		for (vector<int>::iterator it = tmp.begin(); it != tmp.end(); it++) {
			docs[i]->words[n++] = *it;
		}
		tmp.clear();
	}
	n_word = word2id.size();
	fclose(fp);

	return 0;
}