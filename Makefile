CC = g++
CFLAGS = -O3 -Wall

all: lda_cf.o model.o dataset.o utils.o model_tp.o corpus.o
	$(CC) $(CFLAGS) lda_cf.o model.o dataset.o utils.o model_tp.o corpus.o -o bin/lda

lda_cf.o:
	$(CC) $(CFLAGS) -c src/lda_cf.cpp -o lda_cf.o

model.o:
	$(CC) $(CFLAGS) -c src/model.cpp -o model.o

model_tp.o:
	$(CC) $(CFLAGS) -c src/model_tp.cpp -o model_tp.o

dataset.o:
	$(CC) $(CFLAGS) -c src/dataset.cpp -o dataset.o

corpus.o:
	$(CC) $(CFLAGS) -c src/corpus.cpp -o corpus.o

utils.o:
	$(CC) $(CFLAGS) -c src/utils.cpp -o utils.o

clean:
	rm *.o
	
## example ##
#lda -CF -train_file E:/data/epinions/6/train_map.txt -test_file E:/data/epinions/6/test_map.txt -n_user 40163 -n_item 139738 -n_v 5 -n_iter 100 -alpha 0.5 -beta 0.1 -n_topic 10 
#lda -TP -train_file E:/workspace/GibbsLDA++-0.2/data/trndocs.dat -alpha 0.5 -beta 0.1 -n_topic 100 -n_iter 500 -savestep 100 -tpNwd 20 -docNtp 10
#$ lda -CF -train_file E:/data/epinions/6/base_2_map -n_iter 100 -n_user 6040 -n_item 3706 -n_v 5 -test_file E:/data/epinions/6/test_2_map -alpha 1 -beta 0.1 0.1 0.35 0.35 0.1 -n_topic 5
	