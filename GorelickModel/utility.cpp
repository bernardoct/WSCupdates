#include "global.h"

using namespace std;

// This file contains:
// 1. I/O Helper Functions
// 2. Statistics Helper Functions (mean, standard deviation, etc.)
// 3. Memory Management Helper Functions (allocation, zap)

// I/O Helper Functions

void openFile(ifstream &stream, string filename)
{
	stream.open(filename.c_str(), ios::in);
	if (!stream)
	{
		cerr << "Error in reading " << filename << ". Exiting..." << endl;
		exit(-1);
	}
	return;
}

void readFile(data_t &matrix, string filename, int rows, int columns)
{
	ifstream in;
	openFile(in, filename);
	
	matrix.resize(rows, vector<double> (columns));
	
	string tempLine;
	double tempValue = 0.0;
	
	for(int i=0; i < rows; i++) 
	{
		getline(in, tempLine);
		
		stringstream ss(tempLine);
		string field;
		
		for(int j = 0; j < columns; j++)
		{
			getline(ss, field, ',');
			stringstream fs(field);
			fs >> tempValue;

			matrix[i][j] = tempValue;
		}
	}

	in.close();
	return;
}

double average_array_colwise (double **data, int rows, int cols, int col_of_interest)
{
	double dummy_sum = 0.0;
	double average = 0.0;

	for (int row_it = 0; row_it < rows; row_it++)
	{
		dummy_sum = dummy_sum + data[row_it][col_of_interest];
	}

	average = dummy_sum / rows;

	return average;

}

double average_array_colwise (double **data, int rows, int cols, int col_of_interest, int startYear)
{
	double dummy_sum = 0.0;
	double average = 0.0;

	for (int row_it = startYear; row_it < rows; row_it++)
	{
		dummy_sum = dummy_sum + data[row_it][col_of_interest];
	}
	
	average = dummy_sum / (rows-startYear);

	return average;

}

double std_dev_colwise(double **data, int length, int col_of_interest)
{
	double sum = 0.0;

	for (int i = 0; i < length; i++)
	{
		sum += data[i][col_of_interest];
	}

	double mean = sum / length;

	double squared_sum = 0.0;

	for (int i = 0; i < length; i++)
	{
		squared_sum += pow((data[i][col_of_interest]-mean),2.0);
	}

	return sqrt( squared_sum / (length-1) );
}

double std_dev_colwise(double **data, int length, int col_of_interest, int startYear)
{
	double sum = 0.0;

	for (int i = startYear; i < length; i++)
	{
		sum += data[i][col_of_interest];
	}

	double mean = sum / (length-startYear);

	double squared_sum = 0.0;

	for (int i = startYear; i < length; i++)
	{
		squared_sum += pow((data[i][col_of_interest]-mean),2.0);
	}

	return sqrt( squared_sum / (length-1 - startYear) );
}

void index_array(double* indexed_array, const int rows, const int cols, int year_start, double** data)
{
	int a, b;
	for (int x = 0; x<rows; x++)
	{
		a = x*cols;
		b = x+year_start;
		for (int y = 0; y<cols; y++)
			indexed_array[a+y] = data[b][y];
	}
}	

// Memory Management Helper Functions

void general_1d_allocate(double *(&data), int length)
{
	data = new double [length];

	for (int i = 0; i < length; i++) data[i] = 0.0;

	return;
}

void general_1d_allocate(double *(&data), int length, double value)
{
	data = new double [length];

	for (int i = 0; i < length; i++) data[i] = value;

	return;
}

void general_2d_allocate(double **&data, int rows, int cols)
{
	data = new double *[rows];
	for (int row_it = 0; row_it<rows; row_it++) data[row_it] = new double[cols];

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			data[row_it][col_it] = 0.0;
		}
	}
}

void general_2d_allocate(double **&data, int rows, int cols, double value)
{
	data = new double *[rows];
	for (int row_it = 0; row_it<rows; row_it++) data[row_it] = new double[cols];

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			data[row_it][col_it] = value;
		}
	}
}


void general_2d_allocate(int **(&data), int rows, int cols, int init_value)
{
	data = new int *[rows];
	for (int row_it = 0; row_it<rows; row_it++) data[row_it] = new int[cols];

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			data[row_it][col_it] = init_value;
		}
	}
}
