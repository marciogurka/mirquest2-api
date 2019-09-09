//============================================================================
// Name        : boostsvm.cpp
// Author      : Van Du T. Tran
// Version     :
// Copyright   : 
// Description : miRNA in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <fcntl.h>
#include <assert.h>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>

//#include <sys/types.h>
//#include <sys/stat.h>
//#include <stdio.h>

#define NDEBUG
#define DBL_MAX 10e+9
#define DBL_MIN 0

using namespace std;

typedef vector<size_t> iVector;
typedef vector<double> dVector;
typedef vector<int> lVector;
typedef vector<dVector> vVector;
typedef vector<string> sVector;

/*
 * \brief Print a label vector
 */
void printlVector(lVector& v) {
	for (lVector::iterator it=v.begin(); it<v.end(); it++)
		cout<<*it;
	cout<<endl;
}

/*
 * \brief Print an integer vector
 */
void printiVector(ofstream& os, iVector& v) {
	for (iVector::iterator it=v.begin(); it<v.end(); it++)
		os<<*it<<" ";
	os<<endl;
}

/*
 * \brief Print a double vector
 */
void printdvector(ofstream& os, dVector& v) {
	for (dVector::iterator it=v.begin(); it<v.end(); it++)
		os<<*it<<endl;
}

/*
 * \brief Print a string vector
 */
void printsvector(ofstream& os, sVector& v) {
	for (sVector::iterator it=v.begin(); it<v.end(); it++)
		os<<*it<<endl;
}

/*
 * \brief Calculate the sum of double vector elements
 */
double sumdvector(dVector &v) {
	double sum = 0;
	for (dVector::iterator it=v.begin(); it!=v.end(); it++) {
		sum += *it;
	}
	return sum;
}

/*
 * \brief Calculate the sum of integer vector elements
 */
double sumlvector(lVector &v) {
	double sum = 0;
	for (lVector::iterator it=v.begin(); it!=v.end(); it++) {
		sum += *it;
	}
	return sum;
}

/*
 * \brief Pearson correlation
 */
double Pearson(lVector v1, lVector v2) {
	size_t len = v1.size();
	double avg1, avg2, pearson, norm1, norm2;
	
	avg1 = (double)sumlvector(v1)/len;
	avg2 = (double)sumlvector(v2)/len;
	
	pearson = 0;
	norm1 = 0;
	norm2 = 0;
	for (unsigned int i=0; i<len; i++) {
		pearson += (v1.at(i)-avg1)*(v2.at(i)-avg2);
		norm1 += (v1.at(i)-avg1)*(v1.at(i)-avg1);
		norm2 += (v2.at(i)-avg2)*(v2.at(i)-avg2);
	}
	
	return pearson/(sqrt(norm1*norm2));
}

/*
 * \brief Add two double vectors
 */
dVector operator+(const dVector& v1, const dVector& v2) {
	dVector v0;
	for (unsigned int i=0; i<v1.size(); i++)
		v0.push_back(v1.at(i) + v2.at(i));
	return v0;
}

/*
 * \brief Multiply a double boolean vector by a double
 */
dVector operator*(const double& a, const lVector& v) {
	dVector v0;
	for (unsigned int i=0; i<v.size(); i++)
		v0.push_back(a*v.at(i));
	return v0;
}

/*
 * \brief Calculate the squared distance between two vectors v1, v2 stretched at stretch
 */
double squareddistance(double stretch, dVector v1, dVector v2) {
	double res = 0;
	for (unsigned int i=0; i<v1.size(); i++) {
		res += pow(stretch*(v1.at(i)-v2.at(i)), 2);
	}
	return res;
}

/*
 * \brief Calculate the minimum distance in a set of points
 */
double mindistance(double stretch, vVector samples) {
	double res = DBL_MAX, tmp_res;
	for (unsigned int i=0; i<samples.size(); i++)
		for (unsigned int j=i+1; j<samples.size(); j++) {
			tmp_res = squareddistance(stretch, samples.at(i), samples.at(j));
			if (tmp_res < res) {
				res = tmp_res;
				//cout<<res<<endl;
			}
		}
	return sqrt(res);
}

/*
 * \brief Calculate the maximum distance in a set of points
 */
double maxdistance(double stretch, vVector samples) {
	double res = DBL_MIN, tmp_res;
	for (unsigned int i=0; i<samples.size(); i++)
		for (unsigned int j=i+1; j<samples.size(); j++) {
			tmp_res = squareddistance(stretch, samples.at(i), samples.at(j));
			if (tmp_res > res) {
				res = tmp_res;
				//cout<<res<<endl;
			}
		}
	return sqrt(res);
}

/*
 * \brief Convert labels to 0-1 labels
 * \param l Label
 * \return 0-1 label
 */
int standard_label(int l, int positive) {
	if (l == positive)
		return 1;
	return -1;
}

/*
 * \brief Calculate the scatter radius
 */
double scatterradius(double stretch, vVector samples) {
	return maxdistance(stretch, samples);
}

/*
 * \brief Convert data from the 4 existing files to cross validation data and stock in 'samples.test' and 'samples.train'
 * \param data_train Training data file
 * \param label_train Training label file
 * \param data_test Test data file
 * \param label_test Test label file
 * \return Number of training instances iv[0] and number of features iv[1]
 */
iVector convertCVdata(const char *data_train, const char *label_train, const char *data_test, const char *label_test) {
	ifstream input_data_train(data_train), input_label_train(label_train), input_data_test(data_test), input_label_test(label_test);
	ofstream output_file_test("samples.test");
	ofstream output_file_train("samples.train");
	string line, token;
	stringstream sline;
	lVector labels;
	iVector iv;
	int i;

	getline(input_label_train, line);
	while (!input_label_train.eof()) {
		if (atof(line.c_str()) > 0)
			labels.push_back(1);
		else labels.push_back(0);
		getline(input_label_train, line);
	}
	input_label_train.close();
	iv.push_back(labels.size());

	getline(input_data_train, line);
	while (!input_data_train.eof()) {
		output_file_train<<*(labels.begin())<<" ";
		sline.str("");
		sline.clear();
		sline << line;
		i = 0;
		while (sline >> token) {
			output_file_train<<++i<<":"<<token<<" ";
		}
		output_file_train<<endl;
		getline(input_data_train, line);
		labels.erase(labels.begin());
	}
	input_data_train.close();

	getline(input_label_test, line);
	while (!input_label_test.eof()) {
		if (atof(line.c_str()) > 0)
			labels.push_back(1);
		else labels.push_back(0);
		getline(input_label_test, line);
	}
	input_label_test.close();

	getline(input_data_test, line);
	while (!input_data_test.eof()) {
		output_file_test<<*(labels.begin())<<" ";
		sline.str("");
		sline.clear();
		sline << line;
		i = 0;
		while (sline >> token) {
			output_file_test<<++i<<":"<<token<<" ";
		}
		output_file_test<<endl;
		getline(input_data_test, line);
		labels.erase(labels.begin());
	}
	input_data_test.close();

	iv.push_back(i);

	return iv;
}

/*
 * \brief Separate training sample into two parts, one with weight of mu*NB_SAMPLES_TRAIN, one with weight of (1-mu)*NB_SAMPLES_TRAIN
 * \param mu Ratio to discard [0,1]
 */
lVector discard_samples(double mu, int NB_SAMPLES_TRAIN, const char* weights_file, const char* samples_file) {
	ifstream input_file;
	ofstream output_file;
	string line, token;
	stringstream sline;
	double discarded_weight = mu*NB_SAMPLES_TRAIN, new_weight, sum_weight = 0;
	dVector d_weights, r_weights; /** d_: discarded, r_: remaining **/
	sVector samples, d_samples, r_samples;
	sVector labels, r_labels;
	lVector d_labels;
	dVector::iterator dt;
	sVector::iterator st;
	sVector::iterator bt;

	input_file.open(samples_file);
	if (!input_file.is_open()) {
		cerr<<"Cannot open file "<<samples_file<<endl;
		exit(EXIT_FAILURE);
	}
	/** stock all instances in file 'samples_file' into vector 'samples' **/
	for (int j=0; j<NB_SAMPLES_TRAIN; j++) {
		getline(input_file, line);
		samples.push_back(line);
	}
	input_file.close();

	/** create discarded and remaining samples **/
	input_file.open(weights_file);
	if (!input_file.is_open()) {
		cerr<<"Cannot open file "<<weights_file<<endl;
		exit(EXIT_FAILURE);
	}
	double sum = 0;
	for (int j=0; j<NB_SAMPLES_TRAIN; j++) {
		getline(input_file, line);
		new_weight = atof(line.c_str());
		sum += new_weight;
		if (new_weight < discarded_weight) {
			dt = d_weights.begin();
			st = d_samples.begin();
			//bt = d_labels.begin();
			while (dt<d_weights.end() && *dt < new_weight) {
				dt++;
				st++;
				//bt++;
			}

			sum_weight += new_weight;
			d_weights.insert(dt, new_weight);
			d_samples.insert(st, samples.at(j));
			//d_labels.insert(bt, labels.at(j));
			if (sum_weight > discarded_weight) {
				sum_weight -= *(d_weights.end()-1);
				r_weights.push_back(*(d_weights.end()-1));
				d_weights.pop_back();

				r_samples.push_back(*(d_samples.end()-1));
				d_samples.pop_back();
				//r_labels.push_back(*(d_labels.end()-1));
				//d_labels.pop_back();
			}
//			else {
//				sum_weight += new_weight;
//			}
		}
		else {
			r_weights.push_back(new_weight);
			r_samples.push_back(samples.at(j));
			//r_labels.push_back(labels.at(j));
		}
	}
	input_file.close();
	cout<<endl;
	cout<<"Discard "<<d_weights.size()<<" samples of weight "<<sumdvector(d_weights)<<endl;//sum_weight<<endl;
	cout<<"Remain "<<r_weights.size()<<" samples of weight "<<sumdvector(r_weights)<<endl;
	cout<<"All "<<sum<<" with "<<NB_SAMPLES_TRAIN<<" samples"<<endl;

	output_file.open("remains");
	printsvector(output_file, r_samples);
	output_file.close();

	output_file.open("remains.wgt");
	printdvector(output_file, r_weights);
	output_file.close();

	output_file.open("discard");
	printsvector(output_file, d_samples);
	output_file.close();

	for (sVector::iterator it=d_samples.begin(); it<d_samples.end(); it++) {
		sline.str("");
		sline.clear();
		sline << *it;
		sline >> token;
		d_labels.push_back(standard_label(atoi(token.c_str()),1));
	}
	return d_labels;
}

/*
 * \brief Diversity of a component classifier
 * \param
 * \return
 */
bool diversity(bool h, bool f) {
	return (h != f);
}

/*
 * \brief Sign of an integer
 * \param x Integer
 * \return Sign of x
 */
int sign(double x) {
	if (x >= 0) return 1;
	return -1;
}

/*
 * \brief Execute a shell command
 */
void execute(const char *sout, const char *command, char **args) {
	int out, pid, status;

	if ((pid=fork()) == 0) {
	        if (strcmp(sout,"stdout")) {
			out = open(sout, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			dup2(out, 1);
			close(out);
		}
		execvp(command, args);
		cerr<<"Fork failed."<<endl;
		exit(EXIT_FAILURE);
	}
	else {
		waitpid(pid, &status, 0);
	}
}

/* 
 * \brief Random extraction of 'number' elements
 */
iVector extract_random(size_t number, iVector elements) {
	iVector iv;
	iVector::iterator it;
	int j;
	
	//srand(time(NULL));
	
	for (unsigned int i=0; i<number; i++) {
		j = rand()%elements.size();
		it = iv.begin();
		while (it!=iv.end() && *it<elements.at(j))
			it++;
		iv.insert(it, elements.at(j));
		elements.erase(elements.begin()+j);
	}

	return iv;
}

/*
 * \brief Feature subset extraction
 * \param iVector Vector of feature positions being extracted
 */
void extract_feature(iVector elements, const char* iname, const char* oname) {
	ifstream input_file(iname);
	ofstream output_file(oname);
	string line, token;
	stringstream sline;
	unsigned int j, feature_no, colon;
	
	if (!input_file.is_open()) {
		cerr<<"Cannot open file "<<iname<<" ."<<endl;
		exit(EXIT_FAILURE);
	}
	if (!output_file.is_open()) {
		cerr<<"Cannot open file "<<oname<<" ."<<endl;
		exit(EXIT_FAILURE);
	}
	getline(input_file, line);
	while (!input_file.eof()) {
		sline.str("");
		sline.clear();
		sline << line;
		sline >> token;
		output_file << token << "\t";
		
		j = 0;
		while (sline >> token && j < elements.size()) {
			colon = token.find(':');
			feature_no = atoi((token.substr(0,colon)).c_str());
			//cout<<feature_no<<endl;
			if (elements.at(j)==feature_no) {
				output_file << token << "\t";
				j++;
			}
		}
		output_file << endl;
		getline(input_file, line);
	} 
	
	input_file.close();
	output_file.close();
}
//to review: if some feature does not exist

/*
 * \brief Optimal parameters C, g of SVM
 */
// void grid_parameter(char* samples_file, char* weights_file, int k_fold, int Cbegin, int Cend, int Cstep, 
// 					int Gbegin, int Gend, int Gstep) {
// 	stringstream sdouble1, sdouble2;
// 						
// 	for (int c=Cbegin; c<=Cend; c=c+Cstep) {
// 		sdouble1.str("");
// 		sdouble1.clear();
// 		sdouble1 << pow(2,c);
// 		for (int g=Gbegin; g=Gend; g=g+Gstep) {
// 			sdouble2.str("");
// 			sdouble2.clear();
// 			sdouble2 << pow(2,g);
// 			char *args_train[] = {(char*)"svm-train", (char*)"-h", (char*)"0", 
// 								(char*)"-c", (char*)(sdouble1.str()).c_str(), 
// 								(char*)"-g", (char*)(sdouble2.str()).c_str(),
// 								//(char*)"-w1", (char*)"3", (char*)"-w-1", (char*)"1", 
// 								(char*)"-W", (char*)"samples.wgt", 
// 								(char*)"samples.train.scale", NULL};
// 		}
// 	}
// 	
// }

/*
 * \brief Weak classifer construction: training subset method
 */
void weakclassifierSS(int iteration, char *pPATH, int NB_SAMPLES_TRAIN, string **models, double delta) {
	double remain_weight, mu, costpos, costneg;
	stringstream sdouble1, sdouble2, sdouble3, sdouble4, sline;
	ifstream input_file, r_weight_file;
	lVector labels_discard, labels_train, labels_test, svm_labels_train, min_err_svm_labels_train, svm_labels_test;
	dVector r_weights;
	string line, tokenC, tokenG;

	mu = 0.75 - delta;

	/** discard a weight of at most mu*NB_SAMPLES_TRAIN from the training set
	 **/
	labels_discard = discard_samples(mu, NB_SAMPLES_TRAIN, "samples.wgt", "samples.train.scale");
	
	/** remain_weights := vector of weights of remaining samples being trained
	 **/
	r_weights.clear();
	r_weight_file.open("remains.wgt");
	if (!r_weight_file.is_open()) {
		cerr<<"Cannot open file remains.wgt"<<endl;
		exit(EXIT_FAILURE);
	}
	getline(r_weight_file, line);
	while (!r_weight_file.eof()) {
		r_weights.push_back(atof(line.c_str()));
		getline(r_weight_file, line);
	}
	r_weight_file.close();
	remain_weight = 0;
	for (dVector::iterator it=r_weights.begin(); it<r_weights.end(); it++)
		remain_weight += *it;
	cout<<"Remain weight: "<<remain_weight<<" with "<<r_weights.size()<<" samples."<<endl;

	costpos = 1;
	costneg = 1;
	
	sdouble3.str("");
	sdouble3.clear();
	sdouble3 << "remains";
	sdouble3 << iteration;
	sdouble3 << ".model";
	(*models)[iteration] = (sdouble3.str()).c_str();
	cout<<"Model file: "<<(*models)[iteration]<<endl;

	/*******************************************
	 * train RBFSVM on the weighted training set
	 **/
	
	/** search optimal parameters C and g
	 **/
	char *args_grid[] = {(char*)"python", (char*)"libsvm-weights-3.11/tools/grid.py", 
						(char*)"-log2c", (char*)"-1,7,1", (char*)"-log2g", (char*)"-3,0,1", 
						(char*)"-v", (char*)"5", (char*)"remains", NULL};
	execute("opt.param", "python", args_grid);

	/** get optimal parameters from opt.param
	 **/
	input_file.open("opt.param");
	if (!input_file.is_open()) {
		cerr<<"Cannot open opt.param."<<endl;
		exit(EXIT_FAILURE);
	}
	getline(input_file, line);
	input_file.close();
	sline.str("");
	sline.clear();
	sline << line;
	sline >> tokenC;
	sline >> tokenG;
	
	sdouble3.str("");
	sdouble3.clear();
	sdouble3 << costpos;
	sdouble4.str("");
	sdouble4.clear();
	sdouble4 << costneg;
	cout<<(sdouble3.str()).c_str()<<(sdouble4.str()).c_str()<<endl;
	char *args_train[] = {(char*)"svm-train", (char*)"-h", (char*)"0", 
			      (char*)"-c", (char*)(tokenC.c_str()), (char*)"-g", (char*)(tokenG.c_str()),
			      (char*)"-w1", (char*)((sdouble3.str()).c_str()), (char*)"-w-1", (char*)((sdouble4.str()).c_str()),
			      //(char*)"-w1", (char*)"6", (char*)"-w-1", (char*)"1",
			      (char*)"-W", (char*)"remains.wgt", 
			      (char*)"remains", NULL};
	for (int i=0; i<10; i++)
		cout<<args_train[i]<<" ";
	cout<<endl;
	
	sdouble1.str("");
	sdouble1.clear();
	sdouble1 << pPATH;
	sdouble1 << "/libsvm-weights-3.11/svm-train";
	execute("out.train", (sdouble1.str()).c_str(), args_train);

	cout<<"Update model file "<<(*models)[iteration].c_str()<<endl;
	char *args_mv[] = {(char*)"mv", (char*)"remains.model", (char*)((*models)[iteration].c_str()), NULL};
	execute("stdout", "mv", args_mv);						
}

/*
 * \brief Weak classifer construction: feature selection method
 */
void weakclassifierFS(int iteration, char *pPATH, int NB_SAMPLES_TRAIN, string **models, iVector features_no, iVector **features) {
	double costpos, costneg;
	stringstream sline, sdouble1, sdouble3, sdouble4;
	ifstream input_file;
	string line, tokenC, tokenG;
	iVector elements;

	elements = extract_random(5, features_no);
	for (int i=0; i<5; i++)
		cout<<elements.at(i)<<" ";
	cout<<endl;
	/** create samples file with 'elements'-extracted features
	 **/
	extract_feature(elements, "samples.train.scale", "samples.extract");
	
	costpos = 3;
	costneg = 1;
	
	sdouble3.str("");
	sdouble3.clear();
	sdouble3 << "remains";
	sdouble3 << iteration;
	sdouble3 << ".model";
	(*models)[iteration] = (sdouble3.str()).c_str();
	cout<<"Model file: "<<(*models)[iteration]<<endl;

	sdouble3.str("");
	sdouble3.clear();
	sdouble3 << costpos;
	sdouble4.str("");
	sdouble4.clear();
	sdouble4 << costneg;

	/*******************************************
	 * train RBFSVM on the weighted training set
	 **/
	char *args_grid[] = {(char*)"python", (char*)"libsvm-weights-3.11/tools/grid.py", 
						(char*)"-log2c", (char*)"-2,2,1", (char*)"-log2g", (char*)"-3,0,1", 
						(char*)"-v", (char*)"5", (char*)"samples.train.scale", NULL};
	execute("opt.param", "python", args_grid);

	input_file.open("opt.param");
	if (!input_file.is_open()) {
		cerr<<"Cannot open opt.param."<<endl;
		exit(EXIT_FAILURE);
	}
	getline(input_file, line);
	input_file.close();
	
	sline.str("");
	sline.clear();
	sline << line;
	sline >> tokenC;
	sline >> tokenG;
	
	//freopen("test.txt", "w", stdout);
		
	char *args_train[] = {(char*)"svm-train", (char*)"-h", (char*)"0", 
						(char*)"-c", (char*)(tokenC.c_str()), (char*)"-g", (char*)(tokenG.c_str()),
						(char*)"-w1", (char*)"3", (char*)"-w-1", (char*)"1", 
						(char*)"-W", (char*)"samples.wgt", (char*)"samples.train.scale", NULL};
									//(char*)"-w1", (char*)(sdouble3.str()).c_str(), (char*)"-w0", (char*)(sdouble4.str()).c_str(),
	sdouble1.str("");
	sdouble1.clear();
	sdouble1 << pPATH;
	sdouble1 << "/libsvm-weights-3.11/svm-train";
	execute("out.train", (sdouble1.str()).c_str(), args_train);

	cout<<"Update model file "<<(*models)[iteration].c_str()<<endl;
	char *args_mv[] = {(char*)"mv", (char*)"samples.train.scale.model", (char*)((*models)[iteration].c_str()), NULL};
	execute("stdout", "mv", args_mv);
	
	(*features)[iteration] = elements;
}

/*
 * \brief Training
 */
void training(int flag_method, char *pPATH, char *training_file, size_t *NB_ITERATIONS, double **alpha, iVector **features, double delta) {
	string line, token, *models;
	stringstream sline, sdouble;
	size_t iteration, NB_FEATURES, NB_SAMPLES_TRAIN;
	int answer, colon;
	vVector samples_train;
	dVector tmp_vector, decision_train, decision_test, r_weights;
	lVector labels_discard, labels_train, labels_test, svm_labels_train, min_err_svm_labels_train, svm_labels_test;
	ifstream input_file, predict_file;
	ofstream train_file, test_file, weight_file, alpha_file, feature_file;
	double error, weight_sum, *weight;
	iVector features_no;

	input_file.open(training_file);
	if (!input_file.is_open()) {
		cerr<<"Cannot open "<<training_file<<"."<<endl;
		exit(EXIT_FAILURE);
	}
	getline(input_file, line);
	sline.str("");
	sline.clear();
	sline << line;
	sline >> token;
	NB_FEATURES = 0;
	while (sline>>token) {
		colon = token.find(":");
		features_no.push_back(atoi((token.substr(0,colon)).c_str()));
		NB_FEATURES++;
	}
	input_file.close();
	cout<<"Number of features: "<<NB_FEATURES<<endl;
	
	/** scale the training samples to [-1, 1], then stock them in file 'samples.train.scale'
	 **/
	// sdouble.str("");
	// sdouble.clear();
	// sdouble << 1;
	char *args_train_scale[] = {(char*)"svm-scale", 
								//(char*)"-l", (char*)(("-"+sdouble.str()).c_str()), 
								//(char*)"-u", (char*)(sdouble.str()).c_str(), 
								(char*)"-s", (char*)"range", 
								(char*)training_file, NULL};

	sdouble.str("");
	sdouble.clear();
	sdouble << pPATH;
	sdouble << "/libsvm-weights-3.11/svm-scale";
	execute("samples.train.scale", (sdouble.str()).c_str(), args_train_scale);

	/** read file 'samples.train.scale',
	 ** samples_train := vector of training samples
	 ** labels_train := vector of labels of training samples
	 **/
	input_file.open("samples.train.scale");
	if (!input_file.is_open()) {
		cerr<<"Cannot open samples.train.scale file."<<endl;
		exit(EXIT_FAILURE);
	}
	getline(input_file, line);
	while (!input_file.eof()) {
		tmp_vector.clear();
		sline.str("");
		sline.clear();
		sline << line;
		sline >> token;
		answer = standard_label(atoi(token.c_str()),1); // to remove variable answer
		while (sline>>token) {
			colon = token.find(':');
			tmp_vector.push_back(atof((token.substr(colon+1,token.length()-colon)).c_str()));
		}
		samples_train.push_back(tmp_vector);
		labels_train.push_back(answer);
		getline(input_file, line);
	}
	input_file.close();

	NB_SAMPLES_TRAIN = samples_train.size();
	cout<<"Number of training samples: "<<NB_SAMPLES_TRAIN<<endl;

	/** initialize weights to 1, their sum is NB_SAMPLES_TRAIN
	 **/
	weight = new double[NB_SAMPLES_TRAIN];
	for (unsigned int i=0; i<NB_SAMPLES_TRAIN; i++)
		weight[i] = 1;

	error = 0;
	models = new string[*NB_ITERATIONS];
	*alpha = new double[*NB_ITERATIONS];
	*features = new iVector[*NB_ITERATIONS];
	
	/** initialize decision function on training samples
	 **/
	decision_train.clear();
	for (unsigned int i=0; i<NB_SAMPLES_TRAIN; i++)
		decision_train.push_back(0);

	/** open alpha_file containing weights of component classifiers
	 **/
	alpha_file.open("alpha");
	if (!alpha_file.is_open()) {
		cerr<<"Cannot open alpha."<<endl;
		exit(EXIT_FAILURE);
	}
	/** open feature_file containing features used in component classifers
	 **/
	feature_file.open("features");
	if (!feature_file.is_open()) {
		cerr<<"Cannot open features."<<endl;
		exit(EXIT_FAILURE);
	}
	
	/** adaboost iterations
	 **/
	iteration = 0;
	while (iteration < *NB_ITERATIONS) {
		cout<<endl<<"--------------------------------"<<endl;
		cout<<"ITERATION "<<iteration<<endl;

		/** store weights in file 'samples.wgt'
		 **/
		weight_file.open("samples.wgt");
		for (unsigned int i=0; i<NB_SAMPLES_TRAIN; i++)
			weight_file<<weight[i]<<endl;
		weight_file.close();

		/** build weak component classifier
		 **/
		//BUILD_CLASSIFIER:
		if (flag_method==0)
			weakclassifierSS(iteration, pPATH, NB_SAMPLES_TRAIN, &models, delta);
		else weakclassifierFS(iteration, pPATH, NB_SAMPLES_TRAIN, &models, features_no, features);
		
		/** predict the training samples
		 **/
		cout<<"#Predicting on the training set..."<<endl;
		char *args_train_predict[] = {(char*)"svm-predict", (char*)"samples.train.scale", 
									(char*)models[iteration].c_str(), (char*)"samples.train.predict", NULL};
		sdouble.str("");
		sdouble.clear();
		sdouble << pPATH;
		sdouble << "/libsvm-weights-3.11/svm-predict";
		execute("stdout", (sdouble.str()).c_str(), args_train_predict);

		/** svm_labels_train := vector of svm-predicted labels of training samples
		 **/
		svm_labels_train.clear();
		predict_file.open("samples.train.predict");
		if (!predict_file.is_open()) {
			cerr<<"Cannot open file samples.train.predict"<<endl;
			exit(EXIT_FAILURE);
		}
		getline(predict_file, line);
		while (!predict_file.eof()) {
			svm_labels_train.push_back(standard_label(atoi(line.c_str()),1));
			getline(predict_file, line);
		}
		predict_file.close();

		/** ASSERTION **/
		assert(svm_labels_train.size()==labels_train.size());

		/** training error of classifier h[iteration]
		 **/
		error = 0;
		//error_simple = 0;
		for (unsigned int i=0; i<NB_SAMPLES_TRAIN; i++)
			if (labels_train.at(i)!=svm_labels_train.at(i))
				error += weight[i];
		error /= NB_SAMPLES_TRAIN;
		cout<<"Error: "<<error<<" at iteration "<<iteration<<endl;
		
		/** weight of the current component classifier
		 **/
		if (error >= 0.5)
			break;//goto BUILD_CLASSIFIER;
		else if (error > 0)
			(*alpha)[iteration] = 0.5*log(1./error-1);
		else if (error == 0)
			(*alpha)[iteration] = DBL_MAX;
		else break;
		alpha_file<<(*alpha)[iteration]<<endl;
		printiVector(feature_file, (*features)[iteration]);
		
//		cout<<"svmlabelstrain"<<endl;
//		for (lVector::iterator it=svm_labels_train.begin(); it<svm_labels_train.end(); it++)
//			cout<<*it;
//		cout<<endl;

		/** update weights of training samples
		 **/
		for (unsigned int i=0; i<NB_SAMPLES_TRAIN; i++)
			weight[i] *= exp(-(*alpha)[iteration]*standard_label(labels_train.at(i),1)*standard_label(svm_labels_train.at(i),1));

		/** normalize to ensure the total sum is NB_SAMPLES_TRAIN
		 **/
		weight_sum = 0;
		for (unsigned int i=0; i<NB_SAMPLES_TRAIN; i++)
			weight_sum += weight[i];
		for (unsigned int i=0; i<NB_SAMPLES_TRAIN; i++)
			weight[i] *= NB_SAMPLES_TRAIN/weight_sum;
			
//		weight_sum = 0;
//		for (unsigned int i=0; i<nb_samples_train; i++) {
//			weight_sum += weight[i];
//		}
//		cout<<"Weight: "<<weight_sum<<endl;
	
		/** ASSERTION **/
		assert(decision_train.size()==svm_labels_train.size());
		
		/** add predicting results of the current classifier to the decision function 
		 **/
		decision_train = decision_train + (*alpha)[iteration]*svm_labels_train;

		iteration++;
	}
	*NB_ITERATIONS = iteration;
	alpha_file.close();
	feature_file.close();
}

/*
 * \brief Main function
 */
int main(int argc, char **argv) {
	clock_t start = clock();
	size_t NB_SAMPLES_TEST, NB_ITERATIONS;
	lVector labels_train, labels_test, svm_labels_train, svm_labels_test;
	dVector decision_test;
	iVector *features;
	ifstream input_file, predict_file;
	ofstream output_file;
	string line, token;
	stringstream sline, sdouble;
	int answer, flag_method, copt;
	unsigned long int TP, FP, TN, FN;
	double *alpha, delta;
	char *pPATH, *train_file_name, *test_file_name;
	
	srand(time(NULL));
	
	if (argc < 2) {
		cout<<"Usage: miRBoost -i predicting_file [-t training_file] [-n number_of_iterations] [-d delta]"<<endl;
		exit(EXIT_SUCCESS);
	}

	/** location for running boostsvm
	 **/
	pPATH = getenv("PWD");
	//cout<<pPATH<<endl;

	/** default number of iterations
	 **/
	NB_ITERATIONS = 5;
	
	flag_method = 0;
	delta = 0.25;
	
	/** training
	 **/
	opterr = 0;
	train_file_name = NULL;
	while ((copt=getopt(argc, argv, "d:hi:m:n:t:"))!=-1) {
		switch (copt) {
			case 'h':
				cout<<"Usage: miRBoost -i predicting_file [-t training_file] [-n number_of_iterations] [-d delta]"<<endl;
		                exit(EXIT_SUCCESS);
			case 'd':
				delta = atof(optarg);
				break;
			case 'i':
				test_file_name = optarg;
				break;
			case 'm':
    				flag_method = atoi(optarg);
                                if (flag_method==0) cout<<"Training subset method"<<endl;
                                else cout<<"Feature selection method"<<endl;
                                break;
			case 'n':
				NB_ITERATIONS = atoi(optarg);
                                break;
			case 't':
				train_file_name = optarg;
                                break;
			default:
				break;
		}
	}
	if (train_file_name) {
		training(flag_method, pPATH, train_file_name, &NB_ITERATIONS, &alpha, &features, delta);
	}
/*	if (argc >= 5 && strcmp(argv[3],"-t")==0) {
		if (argc >=7 && strcmp(argv[5],"-n")==0) {
			NB_ITERATIONS = atoi(argv[6]);
			if (argc>=9 && strcmp(argv[7],"-m")==0) {
				flag_method = atoi(argv[8]);
				if (flag_method==0) cout<<"Training subset method"<<endl;
				else cout<<"Feature selection method"<<endl;
				if (argc>=11 && strcmp(argv[9],"-d")==0) {
					delta = atof(argv[9]);
				}
			}
		}
		training(flag_method, pPATH, argv[4], &NB_ITERATIONS, &alpha, &features, delta);
	}*/
	/** read alpha weights from alpha_file
	 **/
	else {
		alpha = new double[NB_ITERATIONS];
		input_file.open("alpha");
		if (!input_file.is_open()) {
			cerr<<"Cannot open alpha."<<endl;
			exit(EXIT_FAILURE);
		}
		unsigned int i = 0;
		while (!input_file.eof()) {
			getline(input_file, line);
			alpha[i++] = atof(line.c_str());
		}
		NB_ITERATIONS = i;
		input_file.close();
	}

	/** scale the test samples using the same scale, then stock them in file 'samples.test.scale'
	 **/
	char *args_test_scale[] = {(char*)"svm-scale", (char*)"-r", (char*)"range", test_file_name, NULL};
	sdouble.str("");
	sdouble.clear();
	sdouble << pPATH;
	sdouble << "/libsvm-weights-3.11/svm-scale";
	execute("samples.test.scale", (sdouble.str()).c_str(), args_test_scale);

	/** labels_test := vector of labels of test samples
	 **/
	input_file.open("samples.test.scale");
	if (!input_file.is_open()) {
		cerr<<"Cannot open samples.test.scale file."<<endl;
		exit(EXIT_FAILURE);
	}
	getline(input_file, line);
	while (!input_file.eof()) {
		sline.str("");
		sline.clear();
		sline << line;
		sline >> token;
		answer = standard_label(atoi(token.c_str()),1); // to remove answer variable
		labels_test.push_back(answer);
		getline(input_file, line);
	}
	input_file.close();

	NB_SAMPLES_TEST = labels_test.size();

	/** initialize the decision function on test samples
	 **/
	decision_test.clear();
	for (unsigned int i=0; i<NB_SAMPLES_TEST; i++)
		decision_test.push_back(0);

	for (unsigned int iteration=0; iteration<NB_ITERATIONS; iteration++) {
		sdouble.str("");
		sdouble.clear();
		sdouble << "remains";
		sdouble << iteration;
		sdouble << ".model";
		
		/** predict the test data set
		 ** svm_labels_test := vector of svm-predicted labels of test samples
		 **/
		cout<<endl<<"#Predicting on the test set..."<<endl;
		char *args_test_predict[] = {(char*)"svm-predict", (char*)"samples.test.scale", 
									(char*)((sdouble.str()).c_str()), (char*)"samples.test.predict", NULL};
		sdouble.str("");
		sdouble.clear();
		sdouble << pPATH;
		sdouble << "/libsvm-weights-3.11/svm-predict";
		execute("stdout", (sdouble.str()).c_str(), args_test_predict);

		svm_labels_test.clear();
		predict_file.open("samples.test.predict");
		if (!predict_file.is_open()) {
			cerr<<"Cannot open file samples.test.predict"<<endl;
			exit(EXIT_FAILURE);
		}
		getline(predict_file, line);
		while (!predict_file.eof()) {
			svm_labels_test.push_back(standard_label(atoi(line.c_str()),1));
			getline(predict_file, line);
		}
		predict_file.close();

		/** ASSERTION **/
		assert(svm_labels_test.size()==NB_SAMPLES_TEST);

		/** add predicting results of the current classifier to the decision function 
		 **/
		decision_test = decision_test + alpha[iteration]*svm_labels_test;
		//cout<<iteration<<" "<<alpha[iteration]<<endl;
	}

	/** Result on test data set **/
	cout<<endl<<"#Ensemble prediction on test data set (0: neg, 2: pos):"<<endl;
	cout<<"Predi: ";
	for (unsigned int i=0; i<NB_SAMPLES_TEST; i++)
		cout<<sign(decision_test.at(i))+1;
	cout<<endl;
	cout<<"Label: ";
	for (unsigned int i=0; i<NB_SAMPLES_TEST; i++)
		cout<<labels_test.at(i)+1;
	cout<<endl;

	output_file.open("results." + to_string(getpid()) + ".txt");
	for (unsigned int i=0; i<NB_SAMPLES_TEST; i++)
		output_file<<sign(decision_test.at(i))<<endl;
	output_file.close();

	return 0;

	/** compute score **/
	TP = 0;
	TN = 0;
	FP = 0;
	FN = 0;
	for (unsigned int i=0; i<NB_SAMPLES_TEST; i++) {
		if (sign(decision_test.at(i)) == labels_test.at(i)) {
			if (labels_test.at(i)==1) TP++;
			else TN++;
		}
		else {
			if (labels_test.at(i)==1) FN++;
			else FP++;
		}
	}
	cout<<TP<<" "<<FN<<" "<<TN<<" "<<FP<<endl;
	cout<<"Accuracy: "<<(TP + TN)<<"/"<<NB_SAMPLES_TEST<<" = "<<1.0*(TP+TN)/NB_SAMPLES_TEST<<endl;
	cout<<"Sensitivity: "<<1.0*TP/(TP+FN)<<endl;
	cout<<"Specificity: "<<1.0*TN/(TN+FP)<<endl;
	cout<<"PPV: "<<1.0*TP/(TP+FP)<<endl;
	cout<<"F-score: "<<2.0*(1.0*TP/(TP+FN)*TP/(TP+FP))/(1.0*TP/(TP+FN)+1.0*TP/(TP+FP))<<endl;
	cout<<"MCC: "<<1.0*(TP*TN-FP*FN)/sqrt((TP+FP)*(TP+FN)*(TN+FP)*(TN+FN))<<endl;
	cout<<"G-mean: "<<sqrt(1.0*TP/(TP+FN)*TN/(TN+FP))<<endl;
	/** execution time **/
	cout<<endl<<"Time elapsed: "<<((double)clock()-start)/CLOCKS_PER_SEC<<" seconds"<<endl;
	return 0;
}
