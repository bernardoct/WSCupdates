#include "global.h"
#include "TimeSeriesData.h"

using namespace std;

TimeSeriesData::TimeSeriesData()
{
}

TimeSeriesData::~TimeSeriesData()
{}

void TimeSeriesData::allocate(int inflowR, int inflowC)
{
	numRows = inflowR;
	useSynthetic = false;
	
	general_1d_allocate(averages, inflowC, 0.0);
	general_1d_allocate(standardDeviations, inflowC, 0.0);
	general_2d_allocate(normalizedData, inflowR, inflowC, 0.0);
	general_1d_allocate(averagesAdj, inflowC, 0.0);
	general_1d_allocate(standardDeviationsAdj, inflowC, 0.0);
	general_2d_allocate(normalizedDataAdj, inflowR, inflowC, 0.0);
	general_2d_allocate(rawData, inflowR, inflowC, 0.0);
}

void TimeSeriesData::allocate(int inflowR, int inflowC, int numWeeks, int numRealizations)
{
	numRows = inflowR;
	numRuns = numRealizations;
	useSynthetic = true;
	
	general_1d_allocate(averages, inflowC, 0.0);
	general_1d_allocate(standardDeviations, inflowC, 0.0);
	general_2d_allocate(normalizedData, inflowR, inflowC, 0.0);
	general_1d_allocate(averagesAdj, inflowC, 0.0);
	general_1d_allocate(standardDeviationsAdj, inflowC, 0.0);
	general_2d_allocate(normalizedDataAdj, inflowR, inflowC, 0.0);
	general_2d_allocate(rawData, inflowR, inflowC, 0.0);
	
	general_2d_allocate(simulatedData, numRealizations, numWeeks, 0.0);
	general_2d_allocate(synthetic, numRealizations, numWeeks, 0.0);
}

void TimeSeriesData::calculateNormalizations(int inflowR, int inflowC)
{
	for (int column = 0; column < inflowC; column++)
	{
		averages[column] =  average_array_colwise(rawData, inflowR, inflowC, column);
		standardDeviations[column] = std_dev_colwise(rawData, inflowR, column);

		for (int row = 0; row < inflowR; row++)
			normalizedData[row][column] = (rawData[row][column] - averages[column])/standardDeviations[column];
		
	}
	
	//zap(rawData, inflowR);
	
	return;
}
void TimeSeriesData::calculateNormalizations(int inflowR, int inflowC, int startYear)
{
	for (int column = 0; column < inflowC; column++)
	{
		averages[column] =  average_array_colwise(rawData, inflowR, inflowC, column);
		standardDeviations[column] = std_dev_colwise(rawData, inflowR, column);
		averagesAdj[column] =  average_array_colwise(rawData, inflowR, inflowC, column, startYear);
		standardDeviationsAdj[column] = std_dev_colwise(rawData, inflowR, column, startYear);

		for (int row = 0; row < inflowR; row++)
		{
			normalizedDataAdj[row][column] = (rawData[row][column] - averagesAdj[column])/standardDeviationsAdj[column];
			normalizedData[row][column] = (rawData[row][column] - averages[column])/standardDeviations[column];
		}
		
	}
	
	//zap(rawData, inflowR);
	
	return;
}
