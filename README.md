LDA_CF
======

1. Introduction
===============
	LDA_CF is a c/c++ implementation of URP model(Modeling User Rating Profiles for Collaborative Filtering, Benjamin Marlin) using Gibbs sampling. The method used in the original paper is variation inference. It's more difficult to coding and often not as good as Gibbs method. Since the result is closely related to the initialized process and this process is not easy to implement. Before you go deep into the implementation details, maybe you should read the formula derivation in paper "Regularized Gibbs sampling for user profiling with soft constraints".

2. How to use LDA_CF
====================
  2.1. data format
  ----------------
  The format in training data and test data are the same. As follow:
	  user_id|rating_count
	  user_id item_id rating timestamp
	  user_id item_id rating timestamp
	  ...
	  ...
  The "rating_count" is the number of ratings of user with id "user_id", so there are exactly "rating_count" line below the line "user_id|rating_count". Note that, timestamp is not necessary, it is optional which is specified in the command line by "has_time".
  
  2.2. command line
  -----------------
  lda -CF -train_file <string> -alpha <double> -beta <double> -n_topic <int> -n_iter <int> -n_user <int> -n_item <int> -n_v <int> -test_file <string> -has_time -del <string>
  n_topic: 		the number of topics
  n_iter:		the number of iteration
  n_user:		the number of users
  n_item:		the number of items
  n_v:			the number of difference values of rating score
  train_file:	the absolute path of training file
  test_file:	the absolute path of test file
  has_time:		whether there is timestamp in data set or not, if not, don't write this optional parameter
  del:			delimiter of every rating lines, the default is blank space
  alpha, beta: 	LDA hyper parameters, note that there are "n_v" values for beta
  
  It can also be used for topic model.
  lda -TP -train_file <string> -alpha <double> -beta <double> -eta <double> -n_topic <int> -savestep <int> -tpNwd <int> -docNtp <int>