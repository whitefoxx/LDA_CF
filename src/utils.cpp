#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include "utils.h"
#include "model.h"
#include "model_tp.h"
#include "config.h"

using namespace std;

int utils::parse_args(int argc, char ** argv, model * pmodel) {
    string dir = "";
    string train_file = "";
	string test_file = "";
	string del = " ";
    double alpha = -1.0;
	double eta = -1.0;
    int n_topic = 0;
	int n_iter = 0;
	int n_v = 0;
	int n_user = 0;
	int n_item = 0;
	bool has_time = false;

    for (int i = 0; i < argc; i++) {
		string arg = argv[i];	
		if (arg == "-dir") {
			dir = argv[++i];
		} else if (arg == "-train_file") {
			train_file = argv[++i];
		} else if (arg == "-alpha") {
			alpha = atof(argv[++i]);
		} else if (arg == "-beta") {
			for (int v = 0; v < n_v; v++) {
				pmodel->beta[v] = atof(argv[++i]);
			}
		} else if (arg == "-eta") {
			eta = atof(argv[++i]);	
		} else if (arg == "-n_topic") {
			n_topic = atoi(argv[++i]);
		} else if (arg == "-n_iter") {
			n_iter = atoi(argv[++i]);
		} else if (arg == "-n_user") {
			n_user = atoi(argv[++i]);
		} else if (arg == "-n_item") {
			n_item = atoi(argv[++i]);
		} else if (arg == "-n_v") {
			n_v = atoi(argv[++i]);
			pmodel->beta = new double[n_v];
		} else if (arg == "-test_file") {
			test_file = argv[++i];
		} else if (arg == "-del") {
			del = argv[++i];
		} else if (arg == "-time") {
			if (string(argv[++i]) == "true")
				has_time = true;
		} else {
			// any more?
		}
    }
    
	if (n_topic > 0) pmodel->n_topic = n_topic;
	else return PARAMETER_ERROR;

	if (alpha >= 0.0) pmodel->alpha = alpha;
	else pmodel->alpha = 50.0 / pmodel->n_topic;	// default value for alpha

	if (n_iter > 0) pmodel->n_iter = n_iter;
	else return PARAMETER_ERROR;

	if (n_user > 0) pmodel->n_user = n_user;
	else return PARAMETER_ERROR;

	if (n_item > 0) pmodel->n_item = n_item;
	else return PARAMETER_ERROR;

	if (n_v > 0) pmodel->n_v = n_v;
	else return PARAMETER_ERROR;

	pmodel->del = del;
	pmodel->has_time = has_time;
	
	pmodel->train_file = train_file;
	pmodel->test_file = test_file;
	
	string::size_type idx = train_file.find_last_of("/");			
	if (idx == string::npos) {
	    pmodel->dir = "./";
	} else {
	    pmodel->dir = train_file.substr(0, idx + 1);
	}
	printf("dir = %s\n", pmodel->dir.c_str());
	printf("train_file = %s\n", pmodel->train_file.c_str());
	printf("test_file = %s\n", pmodel->test_file.c_str());
    
    return 0;
}

int utils::parse_args(int argc, char ** argv, model_tp * pmodel) {
	string dir = "";
	string train_file = "";
	double alpha = -1.0;
	double beta = -1.0;
	int n_topic = 0;
	int n_iter = 0;
	int savestep = 0;
	int tpNwd = 0;
	int docNtp = 0;

	for (int i = 0; i < argc; i++) {
		string arg = argv[i];	
		if (arg == "-dir") {
			dir = argv[++i];
		} else if (arg == "-train_file") {
			train_file = argv[++i];
		} else if (arg == "-alpha") {
			alpha = atof(argv[++i]);
		} else if (arg == "-beta") {
			beta = atof(argv[++i]);	
		} else if (arg == "-n_topic") {
			n_topic = atoi(argv[++i]);
		} else if (arg == "-n_iter") {
			n_iter = atoi(argv[++i]);
		} else if (arg == "-savestep") {
			savestep = atoi(argv[++i]);
		} else if (arg == "-tpNwd") {
			tpNwd = atoi(argv[++i]);
		} else if (arg == "-docNtp") {
			docNtp = atoi(argv[++i]);
		} else {
			// any more?
		}
	}

	if (n_topic > 0) pmodel->n_topic = n_topic;
	else return PARAMETER_ERROR;

	if (alpha >= 0.0) pmodel->alpha = alpha;
	else pmodel->alpha = 50.0 / pmodel->n_topic;	// default value for alpha

	if (beta >= 0.0) pmodel->beta = beta;
	else return PARAMETER_ERROR;

	if (n_iter > 0) pmodel->n_iter = n_iter;
	else return PARAMETER_ERROR;

	if (savestep > 0) pmodel->savestep = savestep;
	else return PARAMETER_ERROR;

	if (tpNwd > 0) pmodel->tpNwd = tpNwd;
	else return PARAMETER_ERROR;

	if (docNtp > 0) pmodel->docNtp = docNtp;
	else return PARAMETER_ERROR;

	pmodel->train_file = train_file;

	string::size_type idx = train_file.find_last_of("/");			
	if (idx == string::npos) {
		pmodel->dir = "./";
	} else {
		pmodel->dir = train_file.substr(0, idx + 1);
	}
	printf("dir = %s\n", pmodel->dir.c_str());
	printf("train_file = %s\n", pmodel->train_file.c_str());

	return 0;
}

bool utils::pro_cmp(pair<int, double> a, pair<int, double> b)
{
	return a.second > b.second;
}