#ifndef _global_h
#define _global_h

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

using namespace std;

// defined in utility.cpp

// Define a 2D vector type
typedef vector <vector <double> > data_t;

// I/O Helper Functions (defined in utility.cpp)
void openFile(ifstream &stream, string filename);
void readFile(data_t &matrix, string filename, int rows, int columns);

// Statistics Helper Functions (defined in utility.cpp)
double average_array_colwise (double**, int, int, int);
double average_array_colwise (double**, int, int, int, int);
double std_dev_colwise(double**, int, int);
double std_dev_colwise(double**, int, int, int);

// Making a probability density function from two standardized arrays
void make_PDF(double*, double*, int, int**, double, double);

// Indexing a 2D array to 1D
void index_array(double*, const int, const int, int, double**);

// Memory allocation
void general_1d_allocate(double *(&data), int length);
void general_1d_allocate(double *(&data), int length, double value);
void general_2d_allocate(double **(&data), int rows, int cols);
void general_2d_allocate(double **(&data), int rows, int cols, double value);
void general_2d_allocate(int **(&data), int rows, int cols, int init_value);

#endif
