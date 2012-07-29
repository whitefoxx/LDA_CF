#include "dataset.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <map>
#include "config.h"

int dataset::read_ratings(string data_file, vector<int> *users)
{
	FILE *fp = fopen(data_file.c_str(), "r");
	if (fp == NULL)
		return OPEN_FILE_ERROR;
	char line[100];
	char *tok;
	int user, item, star, timestamp, n;
	while (fgets(line, 100, fp)) {
		tok = strtok(line, "|");
		user = atoi(tok);
		users->push_back(user);
		tok = strtok(NULL, "|");
		n = atoi(tok);	
		user_rs[user] = new rating_set(n);
		double s = 0;
		for (int i = 0; i < n; i++) {
			fgets(line, 100, fp);
			tok = strtok(line, del.c_str());
			user = atoi(tok);
			tok = strtok(NULL, del.c_str());
			item = atoi(tok);
			tok = strtok(NULL, del.c_str());
			star = atoi(tok);
			if (has_time) {
				tok = strtok(NULL, del.c_str());
				timestamp = atoi(tok);
			}
			else {
				timestamp = 0;
			}
			user_rs[user]->ratings[i] = rating(item, star, timestamp);
			s += (double)star;
		}
		user_rs[user]->avg = s / n;
	}
	fclose(fp);

	return 0;
}


int dataset::read_links(string link_file)
{
	FILE *fp = fopen(link_file.c_str(), "r");
	if (fp == NULL)
		return OPEN_FILE_ERROR;
	char line[100];
	char *tok;
	vector<int> *all_links = new vector<int>[n_user];
	int user_1, user_2;
	while (fgets(line, 100, fp)) {
		tok = strtok(line, " ");
		user_1 = atoi(tok);
		tok = strtok(NULL, " ");
		user_2 = atoi(tok);
		all_links[user_1].push_back(user_2);
		all_links[user_2].push_back(user_1);
	}
	fclose(fp);

	for (int i = 0; i < n_user; i++) {
		user_ls[i] = new link_set(all_links[i].size());
		for (int j = 0; j < user_ls[i]->length; j++) {
			user_ls[i]->links[j] = all_links[i][j];
		}
	}

	return 0;
}

