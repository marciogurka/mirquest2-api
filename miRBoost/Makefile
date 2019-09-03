SRC_DIR = src
SVM_DIR = libsvm-weights-3.11
GFOLD_DIR = gFoldmulti

all : clean gFold svm miRBoost

gFold :
	make -C $(GFOLD_DIR)

svm :
	make clean -C $(SVM_DIR)
	make -C $(SVM_DIR)
	sed -e "s#/usr/bin/gnuplot#`which gnuplot`#g" $(SVM_DIR)/tools/grid.py > tmp.py
	mv tmp.py $(SVM_DIR)/tools/grid.py

miRBoost :
	make -C $(SRC_DIR)
	mv $(SRC_DIR)/miRBoost .

clean :
	make clean -C $(SVM_DIR)
	make clean -C $(GFOLD_DIR)
	make clean -C $(SRC_DIR)
	rm -f miRBoost
